#include "../include/montecarlo.hpp"


namespace cleaner{
    montecarlo::montecarlo(world const& w, double epsilon, double gamma, double learning_rate,  int episodes) : w(w), epsilon(epsilon), gamma(gamma), learning_rate(learning_rate), episodes(episodes){
    }

    montecarlo::~montecarlo() = default;

	void montecarlo::plots(){
		//
    }

    void montecarlo::solve(){
      this->init();

      do{
        this->setEpisode();
        this->backup();

        this->plots();
      }while( ++this->cepisode < this->episodes );
    }

    double montecarlo::getValueAt(int s){
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        value = std::max(value, this->qf[s][a]);
      } return value;
    }

    action montecarlo::greedy(int s){
      action agreedy;
      double value = MIN;
      for(int a=0; a<action::END; ++a){
        if( value < this->qf[s][a] ){
          agreedy = static_cast<action>(a);
          value = this->qf[s][a];
        }
      } return agreedy;
    }

    double montecarlo::getReturn(int pose){
      double r = 0;
      for(int i=pose; i<100; i++){
        r += pow( this->gamma, i-pose ) * std::get<2>(this->episode[i]);
      }

      return r;
    }


    void montecarlo::setEpisode(){
      action a;
      double r;
      this->episode.clear();
      int s, ss;

      for(s=0; s<this->w.getNumStates(); ++s){
        for(int a=0; a<action::END; ++a){
          this->pf[s][a] = -1;
        }
      }

      s = 0;
      double rd = rand() / ((double) RAND_MAX);
      for(int i=0; i<100; i++){
        if( rd > this->epsilon ) {
          a = greedy(s);
        }else {
          a = static_cast<action>(rand() % 7);
        }

        w.execute(s, a, ss, r);

        this->episode.emplace_back(std::make_tuple(s, a, r));

        if(this->pf[s][a] == -1){
          this->pf[s][a] = i;
        }

        s = ss;
      }
    }

    void montecarlo::backup(){
      int s, a;
      double cumul;

      for(s=0; s<this->w.getNumStates(); ++s){
        for(a=0; a<action::END; ++a){
          if( this->pf[s][a] > -1 ){
            cumul = this->getReturn(this->pf[s][a]);
            this->jf[s][a].second ++;
            this->jf[s][a].first += cumul;
            this->qf[s][a] = this->jf[s][a].first / this->jf[s][a].second;
              for (int tindex = 0; tindex < w.featuresnb; tindex++) {
                  for (int findex = 0; findex < w.featuresnb; findex++) {
                      double sfeature = w.features(w.getState(s))[findex + a];
                      this->theta[tindex * action::END  + a] = this->theta[tindex * action::END + a] + this->learning_rate * (cumul - sfeature * this->theta[tindex * action::END + a]) * sfeature;
                      featureBackup(s, a);
                  }
              }
          }
        }
      }
    }

    void montecarlo::init(){
      for(int s=0; s<this->w.getNumStates(); ++s){
        this->pf.emplace(s,  std::unordered_map<int, int>());
        this->qf.emplace(s,  std::unordered_map<int, double>());
        this->jf.emplace(s,  std::unordered_map<int, std::pair<double, int>>());
        for(int a=0; a<action::END; ++a){
          this->pf.at(s).emplace(a, -1);
          this->qf.at(s).emplace(a, 0.0);
          this->jf.at(s).emplace(a, std::pair<double, int>(0.0, 0));
        }
      }
        for (int feat = 0; feat < w.featuresnb * action::END ; feat++) {
            this->theta.emplace(feat, 0.0);
        }
    }

    void montecarlo::featureBackup(int s, int a) {
        for (int feat = 0; feat < w.featuresnb; feat++) {
            this->feat_qf[s][a] = this->feat_qf[s][a] + w.features(w.getState(s))[feat * action::END + a] * this->theta[feat * action::END + a];
        }
    }

    void montecarlo::printTheta() {
        for (int index = 0; index < this->theta.size(); index++) {
            std::cout << this->theta[index] << std::endl;
        }
    }

}
