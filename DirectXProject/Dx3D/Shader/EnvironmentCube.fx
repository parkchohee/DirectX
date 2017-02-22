
//
// ���̴� ���� ����
//

float4x4	matWorld;				//���� ���
float4x4	matViewProjection;		//�� ���


textureCUBE Cube_Tex;

samplerCUBE CubeSampler = sampler_state
{
	Texture = (Cube_Tex);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float3 Normal : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
};

//
// ���� ���̴� ��Ʈ�� �Լ�
//
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//������ ���� ��ȯ
	//Input.Position * matWorld = Output.Position;
	Output.Position = mul(Input.Position, matWorld);

	//������ �� ������ȯ
	Output.Position = mul(Output.Position, matViewProjection);

	//�븻 ���� ��ȯ.
	//Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Normal = Input.Normal;

	return Output;

}


//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(float3 Normal : TEXCOORD0) : COLOR
{
	float3 normal = normalize(Normal);

	//Cube �÷� ���ø�
	float4 finalColor = texCUBE(CubeSampler, normal);
	
	return float4( finalColor.rgb, 1 );
}

//
// ��ũ�� ����
//
// Pass ������ �Ѵ�. 
technique ColorShader
{


	//Pass ����
	//�ش� Pass �� ���� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	//�׸��� �ȼ� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	pass p0
	{

		//Pass ������ Device �� SetRenderState ������ �����ϴ�.
		CULLMODE = CW;
		ZWRITEENABLE = FALSE;

		//���� ���̴� ����
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_main();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}


};