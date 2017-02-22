//#include "ModelPixel_HalfLambert.fx"
#include "ModelPixel.fx"


#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 35
#endif



const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;	//�ִ� ��� ũ��
float4x3 amPalette[MATRIX_PALETTE_SIZE_DEFAULT];				//�ִ� ��� ũ�⿡ ���� ��Ĺ迭(���⿡ ����Ǵ� ���� ��İ����� ���´�)


//��Ű�� �Է� ����ü
struct VS_SKIN_INPUT
{
	float4      Position;
	float3      BlendWeights;
	int4        BlendIndices;
	float3		Normal;
	float3		Binormal;
	float3		Tangent;
};

//��Ű�� ��� ����ü
struct VS_SKIN_OUTPUT
{
	float4		Position;			//���� ��ȯ �� ����
	float3		Normal;				//�븻 ��ȯ ��
	float3		Binormal;			//���̳븻 ��ȯ ��
	float3		Tangent;			//ź��Ʈ ��ȯ ��
};




//��Ű�� ó�� �Լ� ( VS_SKIN_INPUT �޾� VS_SKIN_OUTPUT ���Ͻ�Ų�� )
VS_SKIN_OUTPUT VS_Skin(VS_SKIN_INPUT Input, int iNumBones)
{

	//VS_SKIN_OUTPUT Output �� �ʱ�ȭ.
	VS_SKIN_OUTPUT Output = (VS_SKIN_OUTPUT)0;		//ZeroMemory

	float fLastWeight = 1.0;
	float fWeight = 0.0f;
	float afBlendWeights[3] = (float[3])Input.BlendWeights;		//�ش� �������� ����Ǵ� ���� ����ġ
	int aiIndices[4] = (int[4])Input.BlendIndices;				//�ش� �������� ����Ǵ� ���� �ε���

	//���� ��Ű�� ó��.... ( ������ ����޴� ������� ( �ִ� 3�������� )
	for (int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++iBone)
	{
		fWeight = afBlendWeights[iBone];
		fLastWeight -= fWeight;		//����Ǵ� ����ġ ��ŭ (����ġ ���� )
		Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iBone]]) * fWeight;

		Output.Normal += mul(Input.Normal, (float3x3)amPalette[aiIndices[iBone]]) * fWeight;
		Output.Binormal += mul(Input.Binormal, (float3x3)amPalette[aiIndices[iBone]]) * fWeight;
		Output.Tangent += mul(Input.Tangent, (float3x3)amPalette[aiIndices[iBone]]) * fWeight;
	}

	//������� ���� ������ �ϳ� �� ����ġ ���� fLastWeight  �� ���ִ�.

	//������ �ϳ� ó��....
	Output.Position.xyz += mul(Input.Position, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	Output.Normal += mul(Input.Normal, (float3x3)amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	Output.Binormal += mul(Input.Binormal, (float3x3)amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	Output.Tangent += mul(Input.Tangent, (float3x3)amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;

	return Output;
}




//------------------------------------------------------------------------------------
// SkinnedMesh Base 
//------------------------------------------------------------------------------------

//
// ���̴� ���� ����
//
float4x4	matWorld;				//���� ���
float4x4	matViewProjection;		//�� * �������� ���
float3		viewPos;				//������ ��ġ


//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT
{
	float4  Position        : POSITION;			//������ġ
	float3  BlendWeights    : BLENDWEIGHT;		//��������ġ (����3��)
	float4  BlendIndices    : BLENDINDICES;		//����Ǵ� ������ ���ε��� 4��
	float3  Normal          : NORMAL;			//�븻 ����
	float3  Binormal        : BINORMAL;			//���̳븻 ����
	float3  Tangent         : TANGENT;			//ź��Ʈ 
	float2  Texcoord        : TEXCOORD0;		//UV
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float2 Texcoord : TEXCOORD0;	//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 Tangent : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
	float3 Normal : TEXCOORD4;
	float4 ClipPos : TEXCOORD5;
};

//
// ���� ���̴� ��Ʈ�� �Լ�
//
//�Ű������� ������ VS_INPUT ����ü�� ������ ������ ������ �ִ�.
//���� ���̴� �����Լ������� ������ ��ȯ�� ����ϰ� �ȴ�.
//��ȯ�� ���� ������ ���� VS_OUTPUT �� ������ �ǹ��� �ִ�.
VS_OUTPUT vs_main(VS_INPUT Input, uniform int iNumBones )
{
	VS_OUTPUT Output;

	//��Ű�� �Է� ����ü �غ�
	VS_SKIN_INPUT vsi = { 
		Input.Position, 
		Input.BlendWeights, 
		Input.BlendIndices, 
		Input.Normal, 
		Input.Binormal, 
		Input.Tangent };

	//VS_Skin �Լ��� ��Ű�� �Է� ����ü �� �ְ�
	//����Ǵ� ���� ���� ������ ������� VS_SKIN_OUTPUT ���� �޴´�.
	VS_SKIN_OUTPUT vso = VS_Skin(vsi, iNumBones);

	//vsfloat4 worldPos = float4( vso.Position.xyz, 1.0f );o ���� Position �� ���� ��ȯ�� ���� ������ ��ġ�̴�.
	float4 worldPos = float4(vso.Position.xyz, 1.0f);

	//���⼭ ���� ���� �ϴ����.... �� �븻�� ���̳븻 ź��Ʈ�� ���� ��ȯ�� ���� �����̴�.

	//������ �� ������ȯ
	Output.Position = mul(worldPos, matViewProjection);


	//�� ���� �ѱ��.
	Output.ViewDir = viewPos - worldPos.xyz;

	//Texture 
	Output.Texcoord = Input.Texcoord;

	//Tangent, Binormal, Normal ���� �����Ͽ� �ѱ��.
	//Output.Tangent = mul(Input.Tangent, (float3x3)matWorld);
	//Output.Binormal = mul(Input.Binormal, (float3x3)matWorld);
	//Output.Normal = mul(Input.Normal, (float3x3)matWorld);

	//VS_Skin �Լ����� ���� ��ȯ�� �Ȱ��̱� ������ ���Ը��ϸ� �ȴ�.
	Output.Normal = vso.Normal;
	Output.Binormal = vso.Binormal;
	Output.Tangent = vso.Tangent;


	//ȭ�� ���� �ѱ��.
	Output.ClipPos = Output.Position;


	return Output;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int CurNumBones = 1;

VertexShader vsArray20[4] = { compile vs_3_0 vs_main(1),
							  compile vs_3_0 vs_main(2),
							  compile vs_3_0 vs_main(3),
							  compile vs_3_0 vs_main(4) };


//
// ��ũ�� ����
//
// Pass ������ �Ѵ�. 
technique Base
{


	//Pass ����
	//�ش� Pass �� ���� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	//�׸��� �ȼ� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	pass p0
	{
		//���� ���̴� ����
		VertexShader = (vsArray20[CurNumBones]);		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_main();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}
};

technique Shadow
{
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_shadow();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}
};


technique NoiseCutout
{

	//Pass ����
	//�ش� Pass �� ���� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	//�׸��� �ȼ� ���̴��� ���̴� �󸶹������� �������� ���� ��Ʈ���Լ��� ��� �Ǵ��� ����
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_cutout();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}
};
