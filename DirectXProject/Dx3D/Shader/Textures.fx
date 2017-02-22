
//
// ���̴� ���� ����
//

float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���


float tileNum = 2.0f;


//Texture ���� ( �ܺ� ���� )
texture MainTex;

//Sampling ���� ( Pixel Shader ���� ������ ���� )
sampler2D MainSampler = sampler_state{
	Texture = MainTex;		//�� sampler2D �� MainTex �� Sampling �Ѵ�.

	//FilterMode �� ����
	MIPFILTER = ANISOTROPIC;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MAXANISOTROPY = 16;


	//Texture UV ��� ����
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFF00FF;
};




//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD0;
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
};


//
// ���� ���̴� ��Ʈ�� �Լ�
//
//�Ű������� ������ VS_INPUT ����ü�� ������ ������ ������ �ִ�.
//���� ���̴� �����Լ������� ������ ��ȯ�� ����ϰ� �ȴ�.
//��ȯ�� ���� ������ ���� VS_OUTPUT �� ������ �ǹ��� �ִ�.
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//������ ���� ��ȯ
	//Input.Position * matWorld = Output.Position;
	Output.Position = mul(Input.Position, matWorld);

	//������ �� ��ȯ
	Output.Position = mul(Output.Position, matView);

	//���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//�ؽ��� ��ǥ �ѱ��.
	Output.Texcoord = Input.Texcoord * tileNum;

	return Output;

}


struct PS_INPUT{
	float2 Texcoord : TEXCOORD0;
};

//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(PS_INPUT Input) : COLOR
{
	//return float4( Input.Texcoord.x, Input.Texcoord.y, 0, 1 );

	//Texture �� �÷��� ��� �Լ� float4 �� ���ϵ�
	//tex2D(Sampler ����, float2 texture��ǥ);

	float4 finalColor = tex2D(MainSampler, Input.Texcoord);


	return finalColor.bgra;


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