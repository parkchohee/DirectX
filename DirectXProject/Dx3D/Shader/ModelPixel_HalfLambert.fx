
float3 lightDir;			//���� ���� ����

//���� ����
float3 lightColor = float3(1, 1, 1);

//����ŧ�� �Ŀ�
float specPower = 30;				//���ݻ��� ��ī�ο�


//Ambient
float ambient = 0.1f;				//ȯ�汤 ��


//Diffuse Texture
texture Diffuse_Tex;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (Diffuse_Tex);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//Normal Texture
texture Normal_Tex;
sampler2D NormalSampler = sampler_state
{
	Texture = (Normal_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


//Specular Texture
texture Specular_Tex;
sampler2D SpecularSampler = sampler_state
{
	Texture = (Specular_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


//Emission Texture
texture Emission_Tex;
sampler2D EmissionSampler = sampler_state
{
	Texture = (Emission_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


struct PS_INPUT{
	float2 Texcoord : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 Tangent : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
	float3 Normal : TEXCOORD4;

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

	//������ ���� Tangent Space ������ ���͸� ���� ���ͷ� ��ȯ.
	float3x3 TBN = float3x3(
	normalize(Input.Tangent),
	normalize(Input.Binormal),
	normalize(Input.Normal));
	//���� �븻
	float3 worldNormal = mul(tanNormal, TBN);



	//�þ� ���� ����ȭ
	float3 viewDir = normalize(Input.ViewDir);


	//
	// Diffuse Ȯ�걤 ���
	//

	//������ �ٿ����� ���� �븻�� ����
	float d = dot(worldNormal, -lightDir);

	//������ ���� -1 ~ 1 ������ ����Ʈ ���� �����
	//0 ~ 1 �� ������ ����� ( �̰� ��������Ʈ )
	float diff = d * 0.5f + 0.5f;


	//���� Ȯ�걤�� ������
	float3 diffuseLightColor = lightColor * diff;


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
	float3 specularLightColor = lightColor * spec * diff;		//���ݻ簡 Ȯ�걤�� ������ �޴°��


	//
	// ���� Diffuse ����
	//

	float4 diffuseTex = tex2D(DiffuseSampler, Input.Texcoord);
	float3 diffuseColor = diffuseTex.rgb * diffuseLightColor;


	//
	// ���� Specular ����
	//
	float3 specularTex = tex2D(SpecularSampler, Input.Texcoord);
	float3 specularColor = specularTex * specularLightColor;


	//
	// ���� Emission ����
	//
	float3 emissionColor = tex2D(EmissionSampler, Input.Texcoord);


	//���� ������ ����
	float3 finalColor = diffuseColor + specularColor + emissionColor;

	return float4(finalColor, 1);

}
