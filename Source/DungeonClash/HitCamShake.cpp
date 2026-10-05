#include "HitCamShake.h"

UHitCamShake::UHitCamShake()
{
	// set the duration of the camera shake
	OscillationDuration = 0.1f;
	OscillationBlendInTime = 0.01f;
	OscillationBlendOutTime = 0.01f;

	// set the rotation oscillation yaw values
	RotOscillation.Yaw.Amplitude = 3.f;
	RotOscillation.Yaw.Frequency = 1.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Yaw.Waveform = EOscillatorWaveform::PerlinNoise;
}
