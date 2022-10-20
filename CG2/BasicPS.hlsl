#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET{

	float3 light = normalize(float3(1 , -1 , 1));
	float diffuse = saturate(dot(-light , input.normal));
	float brightness = diffuse + 0.3;

	float4 texcolor = float4(tex.Sample(smp , input.uv));
	return float4(texcolor.rgb /** brightness*/ , texcolor.a) * color;

}