struct VSOut
{
	//PS에서 이 순서대로 받는다
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

//constant buffer를 사용하기 위함
cbuffer CBuf
{
	matrix transform;		//matrix : 4 X 4 float
};

VSOut main(float2 pos : POSITION, float3 color : COLOR) //: //SV_POSITION		//System Value
{	
	VSOut vso;
	//mul : 벡터 * 행렬
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	vso.color = color;
	return vso;
}