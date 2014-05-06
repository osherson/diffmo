float smear_factor_lookup(float eta, float error)
{
	float abseta = abs(eta);
	float smear_nominal, smear_up, smear_down;
	float smear_factor;

	if (abseta <= 0.5) {
		smear_nominal = 0.052;
		smear_up = 0.115;
		smear_down = -0.01;
	}
	else if (abseta <= 1.1){
		smear_nominal = 0.057;
		smear_up = 0.114;
		smear_down = 0.001;
	}
	else if (abseta <= 1.7){
		smear_nominal = 0.096;
		smear_up = 0.161;
		smear_down = 0.032;
	}
	else if (abseta <= 2.3){
		smear_nominal = 0.134;
		smear_up = 0.228;
		smear_down = 0.042;
	}
	else if (abseta < 5.0){
		smear_nominal = 0.288;
		smear_up = 0.488;
		smear_down = 0.089;
	}

	if (error < 0.05) smear_factor = smear_down;
	else if (error < 0.15) smear_factor = smear_nominal;
	else if (error < 0.25) smear_factor = smear_up;	

	return smear_factor;	
}
