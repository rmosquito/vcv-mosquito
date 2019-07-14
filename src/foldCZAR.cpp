#include "Mosquito.hpp"
using simd::float_4;


struct FoldCzar : Module {
	enum ParamId {
		
		SKEW_PARAM,
		PITCH_PARAM,
		FOLD_PARAM,
		SWAP_PARAM,
		NUM_PARAMS
	};
	enum InputId {
		PITCH_INPUT,
		SKEW_INPUT,
		FOLD_INPUT,
		SWAP_INPUT,
		NUM_INPUTS
	};
	enum OutputId {
		MAIN_OUTPUT,
		CAT_OUTPUT,
		MTN_OUTPUT,
		NUM_OUTPUTS
	};

	float phase = 0.f;
	float risingPhase = 0.f;
	float fallingPhase = 0.f;
	float skew = 0.5f;
	

	FoldCzar() {
		// Configure the module
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);

		// Configure parameters
		// See engine/Param.hpp for config() arguments
		configParam(PITCH_PARAM, -3.f, 3.f, 0.f, "Pitch", " Hz", 2.f, dsp::FREQ_C4);
		configParam(SKEW_PARAM, 0.01f, 0.99f, 0.5f);
		configParam(FOLD_PARAM, 0.f, 0.99f, 0.9f);
		configParam(SWAP_PARAM, 0.f, 0.99f, 0.5f);
	
	}

	void process(const ProcessArgs &args) override {
		// Implement a simple sine oscillator

		// Compute the frequency from the pitch parameter and input
		float pitch = params[PITCH_PARAM].getValue();
		float skew = params[SKEW_PARAM].getValue();
		float fold = params[FOLD_PARAM].getValue();
		float swap = params[SWAP_PARAM].getValue();

		pitch += inputs[PITCH_INPUT].getVoltage();
		skew += inputs[SKEW_INPUT].getVoltage() / 10;
		fold += inputs[FOLD_INPUT].getVoltage() / 10;
		swap += inputs[SWAP_INPUT].getVoltage() / 10;
		
		pitch = clamp(pitch, -4.f, 4.f);
		// The default pitch is C4 = 261.6256f
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		// Accumulate the phase
		phase += freq * args.sampleTime;


		if (phase >= 1.f) {
			phase -= 1.f;
		}

		// float tri = 1 - simd::fabs(2.0f * (phase - 0.5f) );

		float tri;
			if (phase <= skew)
				tri = 1- (2.0 * (1- (-1 / skew * phase + 1)));
			else
				tri = 1- (2.0 * (1-(1 / (1 - skew) * (phase - skew))));

		float main = tri;
		float cat = tri;
		float mtn = tri;

		if (simd::fabs(tri) > fold)
		{
			// main = simd::sgn(tri) * tri - fold;
			main = simd::sgn(tri) * fold - ( tri - simd::sgn(tri) * fold);
			cat = (simd::sgn(tri) * fold ) + (1 * simd::sgn(tri)) - (tri);
			mtn = tri - ( (simd::sgn(tri) * 1) - tri);
		};

		main = clamp((main / fold), -1.f, 1.f);

		if (simd::fabs(tri) < swap) {
			main = -main;
			mtn = -mtn;
			cat = -cat;
		}
		// float tri = 1 - simd::fabs(2.0f * (phase - 0.5f) );

		outputs[MAIN_OUTPUT].setVoltage(5.f * main);
		outputs[MTN_OUTPUT].setVoltage(5.f * mtn);
		outputs[CAT_OUTPUT].setVoltage(5.f * cat);
}

};


struct FoldCzarWidget : ModuleWidget {
	FoldCzarWidget(FoldCzar *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/fold.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<Rogan3PWhite>(Vec(54, 249), module, FoldCzar::PITCH_PARAM));
		addParam(createParam<Rogan3PWhite>(Vec(54, 184), module, FoldCzar::SKEW_PARAM));
		addParam(createParam<Rogan3PWhite>(Vec(54, 54), module, FoldCzar::FOLD_PARAM));
		addParam(createParam<Rogan3PWhite>(Vec(54, 119), module, FoldCzar::SWAP_PARAM));

		addInput(createInput<PJ301MPort>(Vec(18.5, 259), module, FoldCzar::PITCH_INPUT));
		addInput(createInput<PJ301MPort>(Vec(18.5, 194), module, FoldCzar::SKEW_INPUT));
		addInput(createInput<PJ301MPort>(Vec(18.5, 64), module, FoldCzar::FOLD_INPUT));
		addInput(createInput<PJ301MPort>(Vec(18.5, 129), module, FoldCzar::SWAP_INPUT));

		addOutput(createOutput<PJ301MPort>(Vec(8.5, 323.5), module, FoldCzar::MAIN_OUTPUT));
		addOutput(createOutput<PJ301MPort>(Vec(46.5, 323.5), module, FoldCzar::CAT_OUTPUT));
		addOutput(createOutput<PJ301MPort>(Vec(83.5, 323.5), module, FoldCzar::MTN_OUTPUT));

		}
};


// Define the Model with the Module type, ModuleWidget type, and module slug
Model *modelFoldCzar = createModel<FoldCzar, FoldCzarWidget>("FoldCzar");