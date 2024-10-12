#include <iostream>
#include "header.h"
#include <cmath>
#include <algorithm>
#include <random>

// Fonction de répartition de la loi N(0,1)
double normalcdf(double x) {
  return std::erfc(-x/std::sqrt(2))/2;
}
// Densité de probabilité de la loi N(0,1)
double norm_pdf(const double x) {    
  return (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x);
}
// Générateur de nombre aléatoire suivant la loi N(0,1): Box-Muller Method
double gaussian_box_muller() {         
  double x = 0.0;
  double y = 0.0;
  double euclid_sq = 0.0;
  // on génère deux variables aléatoires uniformes en continue
  // jusqu'à ce que le carré de la distance euclidienne soit inférieur à 1
  do {
    x = 2.0 * rand() / static_cast<double>(RAND_MAX)-1;
    y = 2.0 * rand() / static_cast<double>(RAND_MAX)-1;
    euclid_sq = x*x + y*y;
  } while (euclid_sq >= 1.0);
  // on retourne le nombre généré
  return x*sqrt(-2*log(euclid_sq)/euclid_sq);
}

// Fonctions de la classe black_scholes
  // les constructeurs 
Black_Scholes::Black_Scholes(double S0, double volatility, double riskFreeRate, double t){
  this->S0 = S0;
  this->volatility = volatility;
  this->riskFreeRate = riskFreeRate;
  this->t = t;
}
Black_Scholes::Black_Scholes(const Black_Scholes& Bs){
  this->S0 = Bs.S0;
  this->volatility = Bs.volatility;
  this->riskFreeRate = Bs.riskFreeRate;
  this->t = Bs.t;
}
  //le destructeur
Black_Scholes::~Black_Scholes(){}
  // fonctions  d'accès
double Black_Scholes::get_S0() const {return this->S0;}
double Black_Scholes::get_volatility() const {return this->volatility;}
double Black_Scholes::get_riskFreeRate() const {return this->riskFreeRate;}
double Black_Scholes::get_t() const {return this->t;}
  //les fonctions de modification
void Black_Scholes::set_S0(double S0){this->S0 = S0;}
void Black_Scholes::set_volatility(double volatility){this->volatility = volatility;}
void Black_Scholes::set_riskFreeRate(double riskFreeRate){this->riskFreeRate = riskFreeRate;}
void Black_Scholes::set_t(double t){this->t = t;}

// les fonctions de la classe Option
  // les constructeurs
Option::Option(double K, double T) {
  this->K=K;
  this->T=T;
}
Option::Option(const Option& opt){
  this->K=opt.K;
  this->T=opt.T;
}

// les fonctions de la classe Call_euro
  //fonction de calcul de la valeur d'un call dans le modèle de BSM
double Call_euro::price(const Black_Scholes & bs) {
  double dt=Get_T()-bs.get_t();
  double num=log(bs.get_S0()/Get_K())+(bs.get_riskFreeRate()+pow(bs.get_volatility(),2)*0.5)*dt;
  double denom=bs.get_volatility()*sqrt(dt);
  double d1=num/denom;
  double d2=d1-denom;
  return bs.get_S0()*normalcdf(d1)-exp(-bs.get_riskFreeRate()*dt)*Get_K()*normalcdf(d2);
}
  //Payoff d'un call dans le modèle de BSM
double Call_euro::payoff(double S){
  return std::max(S-Get_K(),0.0);
}
  // fonction de calcul du delta de réplication
double Call_euro::delta(const Black_Scholes & BS) {
  double dt=Get_T()-BS.get_t();  
  double num=log(BS.get_S0()/Get_K())+(BS.get_riskFreeRate()+pow(BS.get_volatility(),2)*0.5)*dt;
  double denom=BS.get_volatility()*sqrt(dt);
  double d1=num/denom;
  return normalcdf(d1);
}

// les fonctions de la classe Put_euro
  // fonction de calcul de la valeur d'un put dans le modèle de BSM
double Put_euro::price(const Black_Scholes &BS) {
    double dt=(Get_T()-BS.get_t());
    double num=log(BS.get_S0()/Get_K())+(BS.get_riskFreeRate()+pow(BS.get_volatility(),2)*0.5)*dt;
    double denom=BS.get_volatility()*sqrt(dt);
    double d1=num/denom;
    double d2=d1-denom;
    return exp(-BS.get_riskFreeRate()*dt)*Get_K()*normalcdf(-d2) - BS.get_S0()*normalcdf(-d1);
}
  //payoff d'un put dans le modèle de BSM
double Put_euro::payoff(double S) {
  return std::max(Get_K()-S,0.0);
}
  // fonction de calcul du delta de réplication
double Put_euro::delta(const Black_Scholes &BS){
  double dt=Get_T()-BS.get_t();  
  double num=log(BS.get_S0()/Get_K())+(BS.get_riskFreeRate()+pow(BS.get_volatility(),2)*0.5)*dt;
  double denom=BS.get_volatility()*sqrt(dt);
  double d1=num/denom;
  return normalcdf(d1) - 1;
}

// fonctions de la classe Montecarlo 
  //les constructeurs
MonteCarlo::MonteCarlo(int simulations){
  this->simulations = simulations;
}
MonteCarlo::MonteCarlo(const MonteCarlo& MC){
  this->simulations = MC.simulations;
}  
  // Fonction de pricing d'une option européenne par la méthode de MC
double MonteCarlo::pricing(Option & opt, const Black_Scholes & Bs) { 
  double U_i; // Valeur aléatoire de la loi normale à la ième simulation
  double S_i = 0.0; // prix du sous-jacent à la date d'exercice lors de l'itération i 
  double sum = 0.0;  // Somme des payoffs 

  for (int i=0; i<get_simulation(); i++) { 
    U_i= gaussian_box_muller();   
    S_i = Bs.get_S0() * exp((Bs.get_riskFreeRate()-pow(Bs.get_volatility(),2)*0.5)*opt.Get_T() + Bs.get_volatility()*sqrt(opt.Get_T())*U_i);   
    sum += opt.payoff(S_i);   
  }
  // on retourne la valeur actualisée de la moyenne des payoff 
  return (exp(-Bs.get_riskFreeRate()*opt.Get_T())*(sum / get_simulation()));
}
  //fonction de calcul du delta par Monte Carlo
double MonteCarlo::delta_MC(Option &opt, const Black_Scholes &Bs, double ds){
  double U_i; // Valeur aléatoire de la loi normale à la ième simulation
  double S_i = 0.0; // prix du sous-jacent à la date d'exercice lors de l'itération i 
  double S_i_ds = 0.0; // prix du sous-jacent+ds à la date d'exercice lors de l'itération i
  double sum = 0.0;  // Somme des payoffs
  double sum_ds = 0.0;  // Somme des payoffs des S_i_ds
  double v = 0.0;  // Valeur de l'option
  double v_ds = 0.0;  // Valeur de l'option suite à une variation du prix du sous ajacent de ds

  //simulation des prix du sous-jacent à la date d'exercice
  for (int i=0; i<get_simulation(); i++) { 
    U_i= gaussian_box_muller();   
    
    S_i = Bs.get_S0() * exp((Bs.get_riskFreeRate()-pow(Bs.get_volatility(),2)*0.5)*opt.Get_T() + Bs.get_volatility()*sqrt(opt.Get_T())*U_i); 
    //simulations des prix du sous-jacent+ds à la date d'exercice en supposant 
    //une variation du prix du sous-jacent de ds point de base
    S_i_ds = (Bs.get_S0() + ds*0.0001) * exp((Bs.get_riskFreeRate()-pow(Bs.get_volatility(),2)*0.5)*opt.Get_T() + Bs.get_volatility()*sqrt(opt.Get_T())*U_i);
    //somme des payoffs
    sum += opt.payoff(S_i); 
    sum_ds += opt.payoff(S_i_ds);
  }
  v = (exp(-Bs.get_riskFreeRate()*opt.Get_T())*(sum / get_simulation()));
  v_ds = (exp(-Bs.get_riskFreeRate()*opt.Get_T())*(sum_ds / get_simulation()));
  //on retourne la valeur du delta en supposant une variation du prix du sous ajacent de ds
  return (v_ds - v)/(ds*0.0001);
}