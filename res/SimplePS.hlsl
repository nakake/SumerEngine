struct VSOutput 
{
	float4 svpos : SV_POSITION;
	float4 color : COLOR;
	float2 uv	 : TEXCOORD;
};

SamplerState	smp			: register(s0);
Texture2D		_MainTex	: register(t0);

float4 pixel(VSOutput input) : SV_TARGET
{
	return _MainTex.Sample(smp, input.uv);
}