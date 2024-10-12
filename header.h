#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#pragma once
// Déclaration de la fonction normalcdf
double normalcdf(double x);
// Déclaration de la fonction norm_pdf
double norm_pdf(double x);
// Déclaration de la fonction gaussian_box_muller
//double generate_normal_distribution(double mean, double stddev);
double gaussian_box_muller();

// Déclaration de la classe Black_scholes
class Black_Scholes{ 
// Déclaration des attributs de la classe Black_scholes
private :
  double S0;
  double volatility;
  double riskFreeRate;
  double t;
public:
// Déclaration des méthodes de la classe Black_scholes
  Black_Scholes(){S0=100; volatility=0.2;riskFreeRate=0.05;t=0;}; // constructeur par défaut
  Black_Scholes(double S0, double volatility, double riskFreeRate, double t);// Constructeur avec paramètres
  Black_Scholes(const Black_Scholes& Bs);//contsructeur par copie
  ~Black_Scholes(); // Destructeur
    //accession aux attributs de la classe Black_scholes
  double get_S0() const;  
  double get_volatility() const;
  double get_riskFreeRate() const;
  double get_t() const;
    //modification des attributs de la classe Black_scholes
  void set_S0(double S0);
  void set_volatility(double volatility);
  void set_riskFreeRate(double riskFreeRate);
  void set_t(double t);
};
     
// Déclaration de la classe des options
class Option{
    // Déclaration des attributs de la classe Option
protected:
  double K,T; //K:le Strike et T: la maturité
    // Déclaration des méthodes de la classe Option
public :
  Option () {K=0.0;T=0;};  //Constructeur par défaut de la classe
  Option (double K, double T );    //Constructeur par paramètre
  Option (const Option & op);  //Constructeur par copie
  ~Option(){};//Destructeur de la classe
  //dans le cadre non standard, le payoff de l'option est utilisé pour le pricing 
  //par les simulations de Monte Carlo, et dans le cas standard les formules 
  //sont dérivées par la formule de Black-Scholes-Merton
  //payoff de l'option ()
  virtual double payoff(double S) {return 0;};// S est le prix du sous-jacent à la date d'exercice de l'option T
  //fonction de pricing viruelle
  virtual double price(const Black_Scholes & Bs) {return 0;};
  //fonction de calcul du delta d'une option
  virtual double delta(const Black_Scholes & Bs) {return 0;};
  //Fonctions pour obtenir les variables 
  double Get_K() const {return K;};
  double Get_T() const {return T;};
  //Fonctions pour modifier les variables 
  void Set_K(double K) {this->K=K;};
  void Set_T(double  T) {this->T=T;};
};
    
// déclaration de la sous classe call européen
class Call_euro: public Option {      
public:
  Call_euro() {K=0;T=0;};  //Constructeur par défaut 
  Call_euro(double K, double T):Option(K,T) {this->K=K;this->T=T;}; //Constructeur par copie
  //Calcule le prix d'un call
  double price(const Black_Scholes & BS) override;
  //Calcule le payoff d'un call européen
  double payoff(double S) override;
  //fonction pour le calcul du delta pour le hedging
  double delta(const Black_Scholes & BS) override; 
  //destructeur de la sous classe call
  ~Call_euro(){};
};

// déclaration de la classe put européen
class Put_euro: public Option {      
public:
  Put_euro() {K=0;T=0;};  //Constructeur par défaut 
  Put_euro(double K, double T):Option(K,T) {this->K=K;this->T=T;}; //Constructeur par copie
  //Calcule le prix d'un put
  double price (const Black_Scholes & BS) override;
  //Calcule le payoff d'une put européen
  double payoff(double S) override; 
  //fonction pour le calcul du delta pour le hedging
  double delta(const Black_Scholes & BS) override; 
  //desctructeur de la sous classe put
  ~Put_euro(){};
};

// déclaration de la classe de simulation de monte carlo
class MonteCarlo {
private:     
  int simulations; // nombre de simulations
public:
  MonteCarlo(){simulations=1000;}; // constructeur par défaut
  MonteCarlo(int simulations); // constructeur par paramètre
  MonteCarlo(const MonteCarlo & MC); // constructeur par copie
  double get_simulation() const {return simulations;}; 
  void Set_simulation(int simulations) {this->simulations=simulations;};
  double pricing(Option & opt, const Black_Scholes & Bs);
  //fonction de calcul du delta avec monte carlo
  double delta_MC(Option & opt, const  Black_Scholes & Bs, double ds); //ds == la variation du prix du sous ajacent
  //cette variation sera considérée en point de base
  //Destructeur de la classe Monte Carlo
  ~MonteCarlo(){}
};

#endif // HEADER_H_INCLUDED