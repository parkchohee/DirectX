
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

//Texture ���� ( �ܺ� ���� )
texture DiffuseTex;

//Sampling ���� ( Pixel Shader ���� ������ ���� )
sampler2D DiffuseSampler = sampler_state{
	Texture = DiffuseTex;		//�� sampler2D �� MainTex �� Sampling �Ѵ�.

	//FilterMode �� ����
	MIPFILTER = ANISOTROPIC;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MAXANISOTROPY = 16;

};


//Texture ���� ( �ܺ� ���� )
texture NormalTex;

//Sampling ���� ( Pixel Shader ���� ������ ���� )
sampler2D NormalSampler = sampler_state{
	Texture = NormalTex;		//�� sampler2D �� MainTex �� Sampling �Ѵ�.

	//FilterMode �� ����
	MIPFILTER = ANISOTROPIC;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MAXANISOTROPY = 16;

};



//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT{
	float4 Position : POSITION;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD0;
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 LightDir : TEXCOORD2;
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

	//�� ���� �ѱ��.
	Output.ViewDir = camPos - worldPos.xyz;

	//Texture 
	Output.Texcoord = Input.Texcoord;

	//Tangent, Binormal, Normal ���� �����Ͽ� ����� �غ��Ѵ�.
	float3 tangent = mul(Input.Tangent, (float3x3)matWorld);
	float3 binormal = mul(Input.Binormal, (float3x3)matWorld);
	float3 normal = mul(Input.Normal, (float3x3)matWorld);

	//��� �����.
	float3x3 TBN = float3x3(
		normalize(tangent),
		normalize(binormal),
		normalize(normal));

	//����� ( ����ȸ������� ������� ��ġ ��İ� ���� )
	float3x3 InvTBN = transpose(TBN);

	Output.LightDir = mul(lightDir, InvTBN);
	Output.ViewDir = mul( normalize(Output.ViewDir), InvTBN);



	return Output;

}


struct PS_INPUT{
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 LightDir : TEXCOORD2;
};

//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(PS_INPUT Input) : COLOR
{
	//�븻 Texture �� �÷��� ������ �´�.
	float3 norTexColor = tex2D(NormalSampler, Input.Texcoord).rgb;

	//ź��Ʈ Space ������ �븻 0 ~ 1 ������ �÷������� -1 �� 1 ���̷�
	//���̴� ������ ���Ϳ� ��Į�� ���⸦ �ϸ� �� ������ �˾Ƽ� ���ش�.
	float3 tanNormal = norTexColor * 2.0f - 1.0f;
	//tanNormal.x = norTexColor.x * 2.0f - 1.0f;
	//tanNormal.y = norTexColor.y * 2.0f - 1.0f;
	//tanNormal.z = norTexColor.z * 2.0f - 1.0f;

	//���� �븻 
	float3 worldNormal = tanNormal;


	//�þ� ���� ����ȭ
	float3 viewDir = normalize(Input.ViewDir);

	//����Ʈ ����ȭ
	float3 lightInTanDir = normalize(Input.LightDir);

	//Texture ���ø� �÷�
	float4 diffColor = tex2D(DiffuseSampler, Input.Texcoord);

	//
	// Diffuse Ȯ�걤 ���
	//

	//������ �ٿ����� ���� �븻�� ����
	float d = dot(worldNormal, -lightInTanDir);

	//0~1 ������ ������ ©��
	float diff = saturate(d);

	//�߻�Ǵ� ���� Ȯ�걤�� ����
	float3 diffuseColor = diffColor.rgb * lightColor * diff;

	//
	// Specular ���ݻ籤 ���
	//

	//������ ���� �븻�� ���ݻ�� ���⺤��
	float3 refl = lightInTanDir + (d * worldNormal * 2.0f);
	refl = normalize(refl);

	//���ݻ� ��
	float spec = dot(refl, viewDir);
	spec = saturate(spec);

	//Power ����
	spec = pow(spec, specPower);

	//�߻�Ǵ� ���� ���ݻ��� ����
	float3 specularColor = lightColor * spec * diff;		//���ݻ簡 Ȯ�걤�� ������ �޴°��

	//���� ������ ����
	float3 finalColor = diffuseColor + specularColor;

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