#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal:NORMAL, float2 uv : TEXCODE) {

	VSOutput output; //ピクセルシェーダーに渡す値
	output.svpos = mul(mat , pos);
	output.normal = normal;
	output.uv = uv;
	
	return output;

}