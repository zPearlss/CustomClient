#pragma once
struct CaretMeasureData {
	int a;
	bool b;

	CaretMeasureData() {
		CaretMeasureData(0xFFFFFFFF, true);
	};
	CaretMeasureData(int param_1, bool param_2) {
		this->a = param_1;
		this->b = param_2;
	};
};