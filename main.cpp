#include <iostream>
#include "header.h"
#include <cmath>
#include <algorithm>
#include <string>  
using namespace std ;

int main() { 
  int n, typ_opt;
  double ds =1;
  double S,sigma,r,strike,maturite;
  Black_Scholes BS(100,0.1,0.05,0); //constructeur par paramètre BS
  cout<<"_____Projet réalisé par__________"<<endl;
  cout<<" "<<endl;
  cout<<"_____FOTSING MAKOU Claudine______"<<endl;
  cout<<"-----SALHI Rim___________________"<<endl;
  cout<<"-----TATOU DEKOU Thibault________"<<endl;
  cout<<" "<<endl;
  cout<<"********----***************** Pricing d'option ***************-----******"<<endl;
  cout<<"   "<<endl;
  cout<<"        ---------------------- Bienvenue au MENU --------------------    "<<endl;
  cout<<"   "<<endl;
  cout<<"*****_____Veuillez saisir les caracteristiques de votre option _____*****"<<endl;
  
  cout<<"Le prix initial du sous-jacent"<<endl; cin>>S; BS.set_S0(S); cout<<"   "<<endl;
  cout<<"La valeur de la volatilité"<<endl; cin>>sigma; BS.set_volatility(sigma);cout<<"   "<<endl;
  cout<<"La valeur du taux d'intérêt sans risque"<<endl; cin>>r; BS.set_riskFreeRate(r); cout<<"   "<<endl;
  cout<<"Le prix d'exercice (strike)"<<endl; cin>>strike; cout<<"   "<<endl;
  cout<<"La maturité de l'option"<<endl; cin>>maturite; cout<<"   "<<endl;
  cout<<"Le type de l'option (call=1 ou put=0)"<<endl; cin>>typ_opt; cout<<"   "<<endl;
  
  Black_Scholes B(BS);  //Constructeur par copie de BS
  Call_euro C(strike,maturite);   
  Put_euro P(strike,maturite);
  MonteCarlo MC(10000);

  //while(n!=0){
    int a;
    cout <<"Souhaitez-vous ?" << endl;
    cout <<"1: Calculer le prix cette option" << endl;
    cout <<"2: Obtenir la strategie de replication" << endl; cin >> a; cout<<"   "<<endl;
    if (a==1){
    cout <<"Par quelle méthode souhaitez-vous faire le pricing ?" << endl;
    cout<<" 1: Formule classique dans le modele de  Black_Scholes_Merton  "<<endl;
    cout<<" 2: La methode de simulation de Monte Carlo "<<endl; cin>>n;cout<<"   "<<endl;
      if (n==1){
        if(typ_opt==1){
          cout<<"Le call dérivé d'un sous-jacent dont : "<<endl;
          cout<<"le prix initial est : "<<B.get_S0()<<endl;
          cout<<"de Volatilité : "<<B.get_volatility()<<endl;
          cout<<"au taux d'interet sans risque: "<<BS.get_riskFreeRate()<<endl;
          cout<<"de prix d'exercice: "<<C.Get_K()<<endl;
          cout<<"avec pour maturité : "<<C.Get_T()<<endl;
          cout<<"Vaut dans le modèle de BSM: "<<C.price(B)<<endl;
        }
        else{
          cout<<"Le put dérivé d'un sous-jacent dont : "<<endl;
          cout<<"le prix initial est : "<<B.get_S0()<<endl;
          cout<<"de Volatilité : "<<B.get_volatility()<<endl;
          cout<<"au taux d'interet sans risque: "<<B.get_riskFreeRate()<<endl;
          cout<<"de prix d'exercice: "<<P.Get_K()<<endl;
          cout<<"avec pour maturité : "<<P.Get_T()<<endl;
          cout<<"Vaut dans le modèle de BSM: "<<P.price(B)<<endl;
        }  
      }
      else{
        if(typ_opt==1){
          cout<<"Le call dérivé d'un sous-jacent dont : "<<endl;
          cout<<"le prix initial est : "<<B.get_S0()<<endl;
          cout<<"de Volatilité : "<<B.get_volatility()<<endl;
          cout<<"au taux d'interet sans risque: "<<BS.get_riskFreeRate()<<endl;
          cout<<"de prix d'exercice: "<<C.Get_K()<<endl;
          cout<<"avec pour maturité : "<<C.Get_T()<<endl;
          cout<<"Vaut par la méthode de Monte carlo: "<<MC.pricing(C,B)<<endl;
        }
        else{
          cout<<"Le put dérivé d'un sous-jacent dont : "<<endl;
          cout<<"le prix initial est : "<<B.get_S0()<<endl;
          cout<<"de Volatilité : "<<B.get_volatility()<<endl;
          cout<<"au taux d'interet sans risque: "<<B.get_riskFreeRate()<<endl;
          cout<<"de prix d'exercice: "<<P.Get_K()<<endl;
          cout<<"avec pour maturité : "<<P.Get_T()<<endl;
          cout<<"Vaut par la méthode de Monte carlo: "<<MC.pricing(P,B)<<endl;
        }
      }
    }
    else{
      if(typ_opt==1){
        cout<<"Le ratio de couverture pour la duplication du portefeuille est: "<<endl;
        cout<<"Dans le modele de BSM: "<<C.delta(B)<<endl;
        cout<<"Par la méthode de Monte Carlo: "<<MC.delta_MC(C, B, ds) <<endl;
      }
      else{
        cout<<"Le ratio de couverture pour la duplication du portefeuille est: "<<endl;
        cout<<"Dans le modele de BSM: "<<P.delta(B)<<endl;
        cout<<"Par la méthode de Monte Carlo: "<<MC.delta_MC(P, B, ds) <<endl;
      }
    }
  return 0;
}