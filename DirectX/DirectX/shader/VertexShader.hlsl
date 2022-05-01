//struct VSOut
//{
//	//PS에서 이 순서대로 받는다
//	float3 color : COLOR;
//	float4 pos : SV_POSITION;
//};

//constant buffer를 사용하기 위함
cbuffer CBuf
{
	matrix transform;		//matrix : 4 X 4 float row_matrix 사용시 조금 느림
};

//다이렉트x 는 행우선
//hlsl는 열 우선

//VSOut main(float3 pos : POSITION, float3 color : COLOR) //: //SV_POSITION		//System Value
//{	
//	VSOut vso;
//	//mul : 벡터 * 행렬
//	vso.pos = mul(transform, float4(pos, 1.0f));
//	vso.color = color;
//	return vso;
//}

float4 main(float3 pos : POSITION) : SV_POSITION		//System Value
{
	return mul(float4(pos, 1.0f), transform);
}