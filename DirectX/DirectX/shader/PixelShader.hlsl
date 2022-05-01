cbuffer CBuf
{
	float4 face_color[6];
};


//float4 main(float3 color : COLOR) : SV_TARGET
//{
//	return float4(color, 1.0f);
//}

float4 main(uint tid : SV_PRIMITIVEID) : Sv_Target
{
	return face_color[tid / 2];
}