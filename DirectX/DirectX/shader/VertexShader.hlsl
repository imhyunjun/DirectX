struct VSOut
{
	//PS���� �� ������� �޴´�
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

//constant buffer�� ����ϱ� ����
cbuffer CBuf
{
	matrix transform;		//matrix : 4 X 4 float
};

VSOut main(float2 pos : POSITION, float3 color : COLOR) //: //SV_POSITION		//System Value
{	
	VSOut vso;
	//mul : ���� * ���
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	vso.color = color;
	return vso;
}