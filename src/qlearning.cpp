#include "../include/qlearning.hpp"


namespace cleaner {
	qlearning::qlearning(world const &w, double epsilon, double learning_rate, double gamma, int episodes) : w(w),
																											 epsilon(epsilon),
																											 learning_rate(
																													 learning_rate),
																											 gamma(gamma),
																											 episodes(
																													 episodes) {}

	qlearning::~qlearning() = default;

	void qlearning::plots() {
		//
	}

	void qlearning::solve() {
		double r;
		int s, a, ss;
		this->init();
		do {
			s = 0;
			for (int i = 0; i < 100; i++) {
				a = greedy(s);
				w.execute(s, static_cast<action>(a), ss, r);
				// this->backup(s, a, ss, r);
				this->eval(s, a, ss, r);
				s = ss;
			}
			this->plots();
		} while (++this->episode < this->episodes);
	}

	double qlearning::getValueAt(int s) {
		double value = MIN;
		for (int a = 0; a < action::END; ++a) {
			value = std::max(value, this->qf[s][a]);
		}
		return value;
	}

	int qlearning::greedy(int s) {
		int agreedy;
		double value = MIN;
		double rd = rand() / ((double) RAND_MAX);

		if (rd > this->epsilon) {
			for (int a = 0; a < action::END; ++a) {
				if (value < this->qf[s][a]) {
					agreedy = a;
					value = this->qf[s][a];
				}
			}
		} else {
			agreedy = rand() % 7;
		}

		return agreedy;
	}

	void qlearning::backup(int s, int a, int ss, double r) {
		this->qf[s][a] =
				this->qf[s][a] + this->learning_rate * (r + this->gamma * this->getValueAt(ss) - this->qf[s][a]);
	}

	void qlearning::featureBackup(int s, int a) {
		for (int feat = 0; feat < w.featuresnb; feat++) {
			this->feat_qf[s][a] = this->feat_qf[s][a] + w.features(w.getState(s))[feat * action::END + a] * this->theta[feat * action::END + a];
		}
	}

	void qlearning::eval(int s, int a, int ss, double r) {

		for (int tindex = 0; tindex < w.featuresnb; tindex++) {
			for (int findex = 0; findex < w.featuresnb; findex++) {
				double ssfeature = w.features(w.getState(ss))[findex * action::END + a];
				double sfeature = w.features(w.getState(s))[findex * action::END + a];
				this->theta[tindex * action::END + a] = this->theta[tindex * action::END + a] + this->learning_rate * (r + this->gamma * ssfeature * this->theta[tindex * action::END + a] - sfeature * this->theta[tindex * action::END + a]) * sfeature;
				featureBackup(s, a);
			}
		}
	}

	double qlearning::getValueFunc(int s) {
		double max = this->qf[s][0];
		for (int a = 1; a < action::END; ++a) {
			max = std::max(max, this->qf[s][a]);
		}
		return max;
	}

	void qlearning::init() {
		for (int s = 0; s < this->w.getNumStates(); ++s) {
			this->qf.emplace(s, std::unordered_map<int, double>());
			this->feat_qf.emplace(s, std::unordered_map<int, double>());
			for (int a = 0; a < action::END; ++a) {
				this->qf.at(s).emplace(a, 0.0);
				this->feat_qf.at(s).emplace(a, 0.0);
			}
		}
		for (int feat = 0; feat < w.featuresnb * action::END ; feat++) {
			this->theta.emplace(feat, 0.0);
		}
	}

	void qlearning::printTheta() {
		for (int index = 0; index < this->theta.size(); index++) {
            std::cout << this->theta[index] << " ";
		}
        std::cout << std::endl;
	}
}
