
#include <iostream>
#include <cmath>
#include "neuron.hpp" 
#include <array> 
#include <fstream> 

using namespace std ; 

const int bufferDelay (2) ;

Neuron::Neuron () : 
Potential (0.0) ,
tau (20.0) , 
I (1.0) ,
time (0.0),
MembraneResistance(20.0) ,
firingThreshold(20),
j (0.1 ), 
BufferCurseur (2)  // entraine un delai de 2 	 
{	spikes.push_back (-2) ; 
	 for (unsigned int i (0) ; i<Buffer.size() ; ++i ) {
		 Buffer [i] = 0 ; 
		 }}
		 
/** 
 * getPotential retourne le potentiel de la membrane du neurone 
 **/
	
double Neuron::getPotential () const {
	return Potential ; 
	}	
	
/**
 * getI retourne l'intensité du courrant exterieur pendant l'intervalle ou ilest sensé arrivé 
 **/
	
double Neuron::getI () const {
	if (debutI <= time and finI >= time ) {
		return I ; }
	else {return 0;	} 
	}/**
 * \class <neuron> [<neuron.hpp>] 
 **/
	
int Neuron::getTime () {
	return time ; }
	
void Neuron::setI (double Z) {
	I = Z ; 
	}
	
vector<double> Neuron::getSpikes () const {
	return spikes ; 
	}
	
double Neuron::getFiringThreshold () const	{
	return firingThreshold ; 
	}
	
void Neuron::setIntervalle (double debut , double fin) {
	debutI = debut ; 
	finI = fin ; 
	}
	
void Neuron::setTime (double t ) {
	time = t ; 
	}
	
/**
 * addConnection crée une connection entre deux neurone en l'ajoutant au vecteur connected 
 **/
 
 // remplacer par une fonction create connection qui fait les connections au hasard avec poisson_gen(vext*ce*h*j) 
	
void Neuron::addConnection (Neuron* n) {
	connected.push_back (n) ; 
	}
	
/**
 *  RefreshPotential actualise le potentiel membranaire a chaqque pas de temps 
 **/

void Neuron::RefreshPotential (double h ) {
	if (isRefractory() ) {Potential = 10.0 ;  }
	if (not isRefractory ()) {
		if (Is_spike() ) {
			Potential = 0 ; } 
		else {
			Potential = exp ( -h * step / tau )*getPotential () + getI()*(MembraneResistance)*(1-exp(-h/tau)) ; }
		Potential += ReceiveSpike () ; }
	vector<double> now ;
	now.push_back (time-1);
	now.push_back (Potential) ;
	Record.push_back (now) ;
	time += 1 ; }

/**
 *  Is_Spike permet de savoir si un spike est crée ou non 
 * @retour 
 * il retourne true si il y a un pic 
 **/
	
bool Neuron::Is_spike (){
	if (Potential >= firingThreshold ) {
		spikes.push_back(time) ; 
		return true ;
		}
	else {return false ; }
	}
	
vector<vector<double> > Neuron::getRecord () const {
	return Record ; 
	} 
	
void Neuron::PrintRecord () const {
	ofstream o ("results.txt") ; 
	for (unsigned int i(0); i<getRecord().size (); ++ i ) {
		o << getRecord()[i] [0]<< "  -> " << getRecord()[i][1] << endl ; }
	}

void Neuron::PrintSpike () const {
	ofstream o ("results.txt") ; 
	if (spikes.size() > 1) {
		o << "we observe "<< spikes.size () - 1 << " spikes at : " << endl ; 
		for (unsigned int i (1); i<spikes.size() ; ++i) {
			o << spikes [i]- 2 << " ms " << endl; 
			}}
	else {o << "there was no spike." << endl ; 
	}}
	
bool Neuron::isRefractory () { 
	double h ; 
	h = spikes.back() ;
	if (time-h < 2.0) {
		return true ;} 
	else { 
return false ; } }

double Neuron::SendSpikes () {
	if (Is_spike ()){
	return j ;}
	else {
		return 0 ; } }
	
double Neuron::ReceiveSpike () {
	++BufferCurseur ;
	double SumSpikes (0.0); 
	for (unsigned int i (0) ; i<connected.size () ; ++i ) { 
		SumSpikes += connected[i]->SendSpikes () ; 
		if (BufferCurseur + bufferDelay < Buffer.size() ) {
			Buffer [BufferCurseur+bufferDelay]= SumSpikes ; }
		if (BufferCurseur + bufferDelay >= Buffer.size ()) {
			Buffer [BufferCurseur + bufferDelay - Buffer.size ()] = SumSpikes ; }
		}
	if (BufferCurseur >= Buffer.size () ) {
		BufferCurseur = 0 ; 
		}
	return Buffer[BufferCurseur] ; 
	 }
