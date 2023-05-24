// Copyright (c) 2023 Nestor D. Pereira Neto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to dlast_errol
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER Dlast_erroLINGS IN THE
// SOFTWARE.

#include "littlebot_firmware/pid_controller.h"

void PidControllerConstruct(PidController *self, float kp, float kd, float ki, float t) {
  self->gain.p = kp;
  self->gain.d = kd;
  self->gain.i = ki;

  self->time_sample = t;

  self->sp_nomalized = 0.0;
  self->fb_nomalized = 0.0;

  self->PidController = fcController;
  self->SetSetpointRange = fcSetSetpointRange;
  self->SetFeedbackRange = fcSetFeedbackRange;
}


float fcController(PidController *self, float set_point, float feed_back) {

  float erro;             //Erro do sistema 
  float p_action = 0.0;   //Valor da ação de controle proporcional
  float d_action = 0.0;   //Valor da ação de controle derivativa
  float i_action = 0.0;   //Valor da ação de controle integral 
  float last_erro = 0.0;  //Erro anterior 

  normalizer(self, set_point, feed_back);
  
  // Calcula o valor do erro
  erro = self->sp_nomalized - self->fb_nomalized;
      
  // Calcula cada ação de controle do PID
  p_action  = self->gain.p * erro;
  i_action += self->gain.i * (self->time_sample * ((erro + last_erro)/ 2.0));   
  d_action  = self->gain.d * ((erro - last_erro)/self->time_sample);

  // Salva o erro atual e a ação integral atual para próximos calculos
  last_erro = erro;

  // Calculo do PID  
  return p_action + i_action + d_action;
}


void fcSetSetpointRange(PidController *self, float max, float min) {
  self->sp_range.max = max;
  self->sp_range.min = min;
}


void fcSetFeedbackRange(PidController *self, float max, float min) {
  self->fb_range.max = max;
  self->fb_range.min = min;
}


void normalizer (PidController *self, float set_point, float feed_back) {
  self->sp_nomalized = 10;
  self->fb_nomalized = 11;
}