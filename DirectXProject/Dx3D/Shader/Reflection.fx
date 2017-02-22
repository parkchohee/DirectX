
//
// ���̴� ���� ����
//

float4x4	matWorld;				//���� ���
float4x4	matViewProjection;		//�� ���

float3 viewPos;					//�� ��ġ

//�ݻ�� ȯ�� ť�� ��
textureCUBE Environment_Tex;
samplerCUBE EnvironmentSampler = sampler_state
{
	Texture = (Environment_Tex);
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
	float3 ViewDir : TEXCOORD1;		//���� ����
};

//
// ���� ���̴� ��Ʈ�� �Լ�
//
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//������ ���� ��ȯ
	float4 worldPos = mul(Input.Position, matWorld);

	//������ �� ������ȯ
	Output.Position = mul(worldPos, matViewProjection);

	//�븻 ���� ��ȯ.
	Output.Normal = mul(Input.Normal, (float3x3)matWorld);

	//�þ� ����
	Output.ViewDir = viewPos - worldPos.xyz;


	return Output;

}


//
// �ȼ� �Է� ����ü
//
struct PS_INPUT{
	float3 Normal : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//���� ����
};


//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
float4 ps_main(PS_INPUT Input) : COLOR
{
	float3 worldNormal = normalize(Input.Normal);
	float3 viewDir = normalize(Input.ViewDir);

	//������ View�� ����
	float3 InDirView = -viewDir;

	//������ View ������ ���� �븻�� �ݻ�
	float3 reflectView = reflect(InDirView, worldNormal);


	//Cube �÷� ���ø�
	float4 finalColor = texCUBE(EnvironmentSampler, reflectView);
	
	return finalColor;
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

		//���� ���̴� ����
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_main();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}


};