float smear_factor_lookup(float eta, float pt_smear_selector)
{
	float abseta = abs(eta);
	float smear_nominal = 0.0;
	float smear_up = 0.0;
	float smear_down = 0.0;
	float smear_factor = 0.0;

	if (abseta <= 0.5) {
		smear_nominal = 0.079;
		smear_up = 0.105;
		smear_down = 0.053;
	}
	else if (abseta <= 1.1){
		smear_nominal = 0.099;
		smear_up = 0.127;
		smear_down = 0.071;
	}
	else if (abseta <= 1.7){
		smear_nominal = 0.121;
		smear_up = 0.150;
		smear_down = 0.092;
	}
	else if (abseta <= 2.3){
		smear_nominal = 0.208;
		smear_up = 0.254;
		smear_down = 0.162;
	}
	else if (abseta <= 2.8){
		smear_nominal = 0.254;
		smear_up = 0.316;
		smear_down = 0.192;
	}
	else if (abseta <= 3.2){
		smear_nominal = 0.395;
		smear_up = 0.458;
		smear_down = 0.332;
	}
	else if (abseta < 5.0){
		smear_nominal = 0.056;
		smear_up = 0.247;
		smear_down = -0.135;
	}

	if (pt_smear_selector < 0.05)      smear_factor = smear_down;
	else if (pt_smear_selector < 0.15) smear_factor = smear_nominal;
	else if (pt_smear_selector < 0.25) smear_factor = smear_up;	

	return smear_factor;	
}
