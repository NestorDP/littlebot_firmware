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
  self->gain_.p = kp;
  self->gain_.d = kd;
  self->gain_.i = ki;

  self->time_sample_ = t;

  self->Controller = fcController;
  self->SetMaxSpeed = fcSetMaxSpeed;
}


float fcController(PidController *self, float set_point, float feed_back) {

  float erro;             //Erro do sistema 
  float p_action = 0.0;   //Valor da ação de controle proporcional
  float d_action = 0.0;   //Valor da ação de controle derivativa
  float i_action = 0.0;   //Valor da ação de controle integral 
  float i = 0.0;
  float d = 0.0;
  float last_erro = 0.0;  //Erro anterior
  float pid = 0.0;
  
  // Calcula o valor do erro normalizado
  erro = (set_point - feed_back)/self->max_speed_;

  // Calculate the I and D
  i += (((erro + last_erro)/ 2.0) * self->time_sample_);
  d  = ((erro - last_erro) / self->time_sample_);

  // Calculate PID actions
  p_action = self->gain_.p * erro;
  i_action = self->gain_.i * i;
  d_action = self->gain_.d * d;

  // Salva o erro atual para próximos calculos
  last_erro = erro;

  // Calculo do PID
  pid = p_action + i_action + d_action;
  if ( pid > 1) {
    pid = 1;
  } else if( pid < 0) {
    pid = 0;
  }
  
  return pid;
}


void fcSetMaxSpeed(PidController *self, float speed) {
  self->max_speed_ = speed;
}
