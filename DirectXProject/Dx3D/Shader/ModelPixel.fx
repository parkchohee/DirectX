

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

//Reflection Texture
texture Reflect_Tex;
sampler2D ReflectSampler = sampler_state
{
	Texture = (Reflect_Tex);
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
	float4 ClipPos : TEXCOORD5;
};



//�ݻ�� ȯ�� ť�� ��
textureCUBE Environment_Tex;
samplerCUBE EnvironmentSampler = sampler_state
{
	Texture = (Environment_Tex);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//��� �÷� ����
struct PS_OUTPUT{
	float4 stage0 : COLOR0;		//0 ���������� ��� �÷�
	float4 stage1 : COLOR1;		//1 ���������� ��� �÷�
	float4 stage2 : COLOR2;		//2 ���������� ��� �÷�
	float4 stage3 : COLOR3;		//3 ���������� ��� �÷�
};



//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
PS_OUTPUT ps_main(PS_INPUT Input) 
{

	PS_OUTPUT Output = (PS_OUTPUT)0;

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

	//�븻 ���� ( x, y ���� ������ �ִ´� )
	float pX = worldNormal.x / (1 + worldNormal.z);
	float pY = worldNormal.y / (1 + worldNormal.z);


	float4 diffuseColor = tex2D(DiffuseSampler, Input.Texcoord);
	float3 specularColor = tex2D(SpecularSampler, Input.Texcoord).rgb;
	float depth = Input.ClipPos.z / Input.ClipPos.w;


	Output.stage0 = float4(diffuseColor.rgb, 0);				//diffuse �÷�.
	Output.stage1 = float4(specularColor, 1);		//specular �÷�.
	Output.stage2 = float4(pX, pY, 0, 0);		//���� �� normal �� ����.
	Output.stage3 = float4(depth, 0, 0, 0);		//depth

	return Output;
}




//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_shadow(PS_INPUT Input) : COLOR0
{
	float depth = Input.ClipPos.z / Input.ClipPos.w;
	return float4(depth, 0, 0, 0);		//depth;
}


PS_OUTPUT ps_tree(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

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

		//�븻 ���� ( x, y ���� ������ �ִ´� )
		float pX = worldNormal.x / (1 + worldNormal.z);
	float pY = worldNormal.y / (1 + worldNormal.z);


	float4 diffuseColor = tex2D(DiffuseSampler, Input.Texcoord);
		float3 specularColor = tex2D(SpecularSampler, Input.Texcoord).rgb;
		float depth = Input.ClipPos.z / Input.ClipPos.w;


	Output.stage0 = float4(diffuseColor.rgb, 0);				//diffuse �÷�.
	Output.stage1 = float4(specularColor, 1);		//specular �÷�.
	Output.stage2 = float4(pX, pY, 0, 0);		//���� �� normal �� ����.
	Output.stage3 = float4(depth, 0, 0, 0);		//depth


	//clip( ��Į�� )		// �ԷµǴ� ��Į���� ������ �ȼ���ü�� �ȱ׸���.
	clip(diffuseColor.a - 0.5f);

	return Output;
}



//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_shadowTree(PS_INPUT Input) : COLOR0
{
	float4 diffuseColor = tex2D(DiffuseSampler, Input.Texcoord);

	float depth = Input.ClipPos.z / Input.ClipPos.w;
	
	//clip( ��Į�� )		// �ԷµǴ� ��Į���� ������ �ȼ���ü�� �ȱ׸���.
	clip(diffuseColor.a - 0.5f);

	return float4(depth, 0, 0, 0);		//depth;
}



//
// ���� ������� ��
//////////////////////////////////////

//Noise Texture
texture Noise_Tex;
sampler2D NoiseSampler = sampler_state
{
	Texture = (Noise_Tex);
	MAGFILTER = POINT;
	MINFILTER = POINT;
	MIPFILTER = POINT;
};

float clipAmount = 0.0f;

float4 clipLineColor = float4(0, 1, 1, 1);


PS_OUTPUT ps_cutout(PS_INPUT Input)
{

	PS_OUTPUT Output = (PS_OUTPUT)0;

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

		//�븻 ���� ( x, y ���� ������ �ִ´� )
		float pX = worldNormal.x / (1 + worldNormal.z);
	float pY = worldNormal.y / (1 + worldNormal.z);


	float4 diffuseColor = tex2D(DiffuseSampler, Input.Texcoord);
		float3 specularColor = tex2D(SpecularSampler, Input.Texcoord).rgb;
		float depth = Input.ClipPos.z / Input.ClipPos.w;


	Output.stage0 = float4(diffuseColor.rgb, 0);				//diffuse �÷�.
	Output.stage1 = float4(specularColor, 1);		//specular �÷�.
	Output.stage2 = float4(pX, pY, 0, 0);		//���� �� normal �� ����.
	Output.stage3 = float4(depth, 0, 0, 0);		//depth



	//������ texture r ���� ���ø��Ѵ�.
	float clipRed = tex2D(NoiseSampler, Input.Texcoord).r;

	float clipValue = clipRed - clipAmount;

	//©���� ��迡�ִ�.
	if (clipValue >= 0.0f  && clipValue <= 0.03f){
		Output.stage0 = clipLineColor;
	}
	clip(clipRed - clipAmount);

	return Output;
}


float4 ps_shadowcutout(PS_INPUT Input) : COLOR0
{
	float depth = Input.ClipPos.z / Input.ClipPos.w;

	//������ texture r ���� ���ø��Ѵ�.
	float clipRed = tex2D(NoiseSampler, Input.Texcoord).r;

	float clipValue = clipRed - clipAmount;

	clip(clipRed - clipAmount);


	return float4(depth, 0, 0, 0);		//depth;
}


