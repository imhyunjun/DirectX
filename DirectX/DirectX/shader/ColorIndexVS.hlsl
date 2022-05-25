//struct VSOut
//{
//	//PS���� �� ������� �޴´�
//	float3 color : COLOR;
//	float4 pos : SV_POSITION;
//};

//constant buffer�� ����ϱ� ����
cbuffer CBuf
{
	matrix transform;		//matrix : 4 X 4 float row_matrix ���� ���� ����
};

//���̷�Ʈx �� ��켱
//hlsl�� �� �켱

//VSOut main(float3 pos : POSITION, float3 color : COLOR) //: //SV_POSITION		//System Value
//{	
//	VSOut vso;
//	//mul : ���� * ���
//	vso.pos = mul(transform, float4(pos, 1.0f));
//	vso.color = color;
//	return vso;
//}

float4 main(float3 pos : POSITION) : SV_POSITION		//System Value
{
	return mul(float4(pos, 1.0f), transform);
}