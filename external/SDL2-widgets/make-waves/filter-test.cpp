/*  
    Demo program for SDL-widgets-2.1
    To be included in make-waves.cpp
    Copyright 2011-2013 W.Boeke

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program.
*/

#include <math.h>
const int TESTSAMPLES=SAMPLE_RATE/10; // so 20Hz will be tested 2 times

struct FilterBase {
  int mode;
  FilterBase():mode(0) {
  }
  virtual void init(float cutoff,float fq,int slider_val,char **txt)=0;
  virtual float getSample(float input)=0;
  virtual const char *get_mode(int nr)=0;
};

struct Filter2:FilterBase {
  float freqcut,fc1,fc2,
        d1,d2,d3,d4,
        hp,
        qres;
  enum { Lopass,Bandpass,Hipass };
  const char *get_mode(int nr) {
    if (nr>=3) return 0;
    static const char *mm[7]={ "lowpass","bandpass","highpass" };
    return mm[nr];
  }
  void init(float cutoff,float fq,int sl_val,char **txt) {
    freqcut=2 * M_PI * cutoff / SAMPLE_RATE;
    fc1=fc2=freqcut;
    //fc1=freqcut/1.1; fc2=freqcut*1.1; // <-- flatter peak
    d1=d2=d3=d4=0;
    static float filter_q[8]={ 1.4,1.0,0.6,0.4,0.3,0.2,0.14,0.1 };
    qres=filter_q[sl_val];
    set_text(*txt,"%.1f",1./qres);
  }
  float getSample(float input) {
    float highpass,output;
    d2+=fc1*d1;                // d2 = lowpass
    highpass=input-d2-qres*d1;
    d1+=fc1*(input-d2-qres*d1); // d1 = bandpass
    output= mode==Lopass ? d2 : mode==Bandpass ? d1 : mode==Hipass ? highpass : 0;

    d4+=fc2*d3;                // d4 = lowpass
    highpass=output-d4-qres*d3;
    d3+=fc2*highpass;    // d3 = bandpass
    output= mode==Lopass ? d4 : mode==Bandpass ? d3 : highpass;
    return output;
  }
};

struct Biquad:FilterBase {
  float a0, a1, a2, a3, a4,
        x1, x2, y1, y2;
  const char *get_mode(int nr) {
    if (nr>=6) return 0;
    static const char *mm[7]={  "LP", "BP", "HP", "notch", "EQ-H", "EQ-L" };
    return mm[nr];
  }
  void init(float cutoff,float fq,int,char **txt) {
    set_text(*txt,0);
    enum {  LPF, BPF, HPF, NOTCH, PEQ_H, PEQ_L };
    float omega, sn, cs, alpha,
          A0, A1, A2, B0, B1, B2;
    /* setup variables */
    omega = 2 * M_PI * cutoff / SAMPLE_RATE;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn * fq / 2.; // filter Q

    switch (mode) {
    case LPF:
        B0 = (1 - cs) /2;
        B1 = 1 - cs;
        B2 = (1 - cs) /2;
        A0 = 1 + alpha;
        A1 = -2 * cs;
        A2 = 1 - alpha;
        break;
    case HPF:
        B0 = (1 + cs) /2;
        B1 = -(1 + cs);
        B2 = (1 + cs) /2;
        A0 = 1 + alpha;
        A1 = -2 * cs;
        A2 = 1 - alpha;
        break;
    case BPF:
        B0 = alpha;
        B1 = 0;
        B2 = -alpha;
        A0 = 1 + alpha;
        A1 = -2 * cs;
        A2 = 1 - alpha;
        break;
    case NOTCH:
        B0 = 1;
        B1 = -2 * cs;
        B2 = 1;
        A0 = 1 + alpha;
        A1 = -2 * cs;
        A2 = 1 - alpha;
        break;
    case PEQ_H:
    case PEQ_L: {
        float A= mode==PEQ_H ? 2. : 0.5;
        B0 = 1 + (alpha * A);
        B1 = -2 * cs;
        B2 = 1 - (alpha * A);
        A0 = 1 + (alpha /A);
        A1 = -2 * cs;
        A2 = 1 - (alpha /A);
      }
      break;
/*
    case LSH:
        B0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
        B1 = 2 * A * ((A - 1) - (A + 1) * cs);
        B2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
        A0 = (A + 1) + (A - 1) * cs + beta * sn;
        A1 = -2 * ((A - 1) + (A + 1) * cs);
        A2 = (A + 1) + (A - 1) * cs - beta * sn;
        break;
    case HSH:
        B0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        B1 = -2 * A * ((A - 1) + (A + 1) * cs);
        B2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        A0 = (A + 1) - (A - 1) * cs + beta * sn;
        A1 = 2 * ((A - 1) - (A + 1) * cs);
        A2 = (A + 1) - (A - 1) * cs - beta * sn;
        break;
*/
     default:
        alert("mode?");
        B0=B1=B2=A0=A1=A2=0; // make compiler happy
    }

    /* precompute the coefficients */
    a0 = B0 /A0;
    a1 = B1 /A0;
    a2 = B2 /A0;
    a3 = A1 /A0;
    a4 = A2 /A0;

    /* zero initial samples */
    x1 = x2 = 0;
    y1 = y2 = 0;
  }
  float getSample(float sample) {
    float result=
      a0 * sample + a1 * x1 + a2 * x2 - a3 * y1 - a4 * y2;
    x2 = x1;
    x1 = sample;
    y2 = y1;
    y1 = result;
    return result;
  }
};

struct PinkNoise:FilterBase { // oversampled
  float b0,b1,b2,
        prev,out;
  const char *get_mode(int nr) { 
    if (nr>=3) return 0;
    static const char *mm[7]={ "2 sections","3 sections","oversampled" };
    return mm[nr];
  }
  void init(float cutoff,float fq,int,char **txt) {
    b0=b1=b2=prev=out=0.;
  }
  float getSample(float input) {
    input= input/4. - out/20.;  // scaling and DC blocker
    if (mode==0) {  // 2 sections
      b1 = 0.98 * b1 + input * 0.4;
      b2 = 0.6 * b2 + input;
      out = b1 + b2;
    }
    else {          // 3 sections
      if (mode==2) {             // oversampling
        float pr=(input+prev)/2; // interpolation
        prev=input;
        b0 = 0.998 * b0 + pr * 0.1;
        b1 = 0.963 * b1 + pr * 0.3;
        b2 = 0.57 * b2 + pr;
      }
      b0 = 0.998 * b0 + input * 0.1;
      b1 = 0.963 * b1 + input * 0.3;
      b2 = 0.57 * b2 + input;
      out= b0 + b1 + b2;
    }
    return out;
  }
};  

/* from sinc.c:
3 points: 1.00 0.64 (0.44 0.28)
4 points: 1.00 0.50 (0.33 0.17)
5 points: 1.00 0.83 0.41 (0.29 0.24 0.12)
6 points: 1.00 0.72 0.35 (0.24 0.17 0.08)
*/

struct ThreePoint:FilterBase {
  float d1,d2,d3,d4,out,
        f_q;
  const char *get_mode(int nr) { 
    if (nr>=3) return 0;
    static const char *name[3]={ "2 points","3 points","4 points" };
    return name[nr];
  }
  void init(float cutoff,float fq,int sl_val,char **txt) {
    d1=d2=d3=d4=out=0;
    const float fq_arr[6]={ 0,0.4,0.7,1.,1.4,1.7 };
    f_q=fq_arr[sl_val];
    set_text(*txt,"%.1f",f_q);
  }
  float getSample(float input) {
    switch (mode) {
      case 0:
        out=((d1=d2)+(d2=input-f_q*out))*0.5;
/*
        d1=d2; d2=input-f_q*out;
        out=(d1+d2)*0.5;
*/
        break;
      case 1:
        d1=d2; d2=d3; d3=input-f_q*out;
        out=0.28*(d1+d3) + 0.44*d2;
        break;
      case 2:
        d1=d2; d2=d3; d3=d4; d4=input-f_q*out;
        out=0.17*(d1+d4) + 0.33*(d2 + d3);
        break;
    }
    return 2*out;
  }
};
