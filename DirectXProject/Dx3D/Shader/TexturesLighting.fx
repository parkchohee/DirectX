
//
// ���̴� ���� ����
//

float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���



float3 lightDir;			//���� ���� ����
float3 camPos;				//������ ��ġ

//����ŧ�� �Ŀ�
float specPower = 30;				//���ݻ��� ��ī�ο�

//���� ����
float3 lightColor = float3(1, 1, 1);

//Emissive �÷�
float3 emissiveColor = float3(0, 1, 1);


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



//Texture ���� ( �ܺ� ���� )
texture SpecTex;

//Sampling ���� ( Pixel Shader ���� ������ ���� )
sampler2D SpecSampler = sampler_state{
	Texture = SpecTex;		//�� sampler2D �� MainTex �� Sampling �Ѵ�.

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




//Texture ���� ( �ܺ� ���� )
texture EmisTex;

//Sampling ���� ( Pixel Shader ���� ������ ���� )
sampler2D EmisSampler = sampler_state{
	Texture = EmisTex;		//�� sampler2D �� MainTex �� Sampling �Ѵ�.

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
	float3 Normal : NORMAL;
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 Normal : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD2;		//�̰� �ȼ����̴��� ���޵ȴ�.
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
	float4 worldPos = mul(Input.Position, matWorld);

	//������ �� ��ȯ
	Output.Position = mul(worldPos, matView);

	//���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//�Էµ� �븻 ���� ���� ������ ��������.
	float3 worldNormal = mul(Input.Normal, (float3x3)matWorld);

	//���� �븻 �ѱ��.
	Output.Normal = worldNormal;

	//�� ���� �ѱ��.
	Output.ViewDir = camPos - worldPos.xyz;

	//Texture 
	Output.Texcoord = Input.Texcoord;

	return Output;

}


struct PS_INPUT{
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 Normal : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD2;		//�̰� �ȼ����̴��� ���޵ȴ�.
};

//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(PS_INPUT Input) : COLOR
{

	//Texture ���ø� �÷�
	float4 texColor = tex2D(MainSampler, Input.Texcoord);
	float4 specTexColor = tex2D(SpecSampler, Input.Texcoord);
	float4 emisTexColor = tex2D(EmisSampler, Input.Texcoord);



	//���� �븻 ����ȭ
	float3 worldNormal = normalize(Input.Normal);

	//�þ� ���� ����ȭ
	float3 viewDir = normalize(Input.ViewDir);


	//
	// Diffuse Ȯ�걤 ���
	//

	//������ �ٿ����� ���� �븻�� ����
	float d = dot(worldNormal, -lightDir);

	//0~1 ������ ������ ©��
	float diff = saturate(d);

	//�߻�Ǵ� ���� Ȯ�걤�� ����
	float3 diffColor = texColor.rgb * lightColor * diff;

	//
	// Specular ���ݻ籤 ���
	//

	//������ ���� �븻�� ���ݻ�� ���⺤��
	float3 refl = lightDir + (d * worldNormal * 2.0f);
	refl = normalize(refl);

	//���ݻ� ��
	float spec = dot(refl, viewDir);
	spec = saturate(spec);

	//Power ����
	spec = pow(spec, specPower);

	//�߻�Ǵ� ���� ���ݻ��� ����
	float3 specColor = (specTexColor.rgb * lightColor)* spec * diff;		//���ݻ簡 Ȯ�걤�� ������ �޴°��


	float3 emiColor = emisTexColor.rgb * emissiveColor;

	//���� ������ ����
	//float3 finalColor = diffColor + specColor + ambient;
	float3 finalColor = diffColor + specColor + emiColor;

	return float4(finalColor, 1);


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