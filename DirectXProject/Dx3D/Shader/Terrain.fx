

//
// ���̴� ���� ����
//
float4x4	matViewProjection;	//������ ���

//���� ���� ����
float3 lightDir = float3( 0, -1, 0);			

//���� ����
float3 lightColor = float3(1, 1, 1);

//����ŧ�� �Ŀ�
float specPower = 30;				//���ݻ��� ��ī�ο�

//Ambient
float ambient = 0.1f;				//ȯ�汤 ��



texture Splat_Tex;
sampler2D Splat_Sampler = sampler_state
{
	Texture = (Splat_Tex);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};



//Diffuse Texture
texture Tile_Diffuse_0;
sampler2D Tile_Sampler_0 = sampler_state{
	Texture = (Tile_Diffuse_0);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Diffuse_1;
sampler2D Tile_Sampler_1 = sampler_state
{
	Texture = (Tile_Diffuse_1);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


texture Tile_Diffuse_2;
sampler2D Tile_Sampler_2 = sampler_state
{
	Texture = (Tile_Diffuse_2);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Diffuse_3;
sampler2D Tile_Sampler_3 = sampler_state
{
	Texture = (Tile_Diffuse_3);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//Normal Texture
texture Tile_Normal_0;
sampler2D Tile_NormalSampler_0 = sampler_state{
	Texture = (Tile_Normal_0);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Normal_1;
sampler2D Tile_NormalSampler_1 = sampler_state{
	Texture = (Tile_Normal_1);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Normal_2;
sampler2D Tile_NormalSampler_2 = sampler_state{
	Texture = (Tile_Normal_2);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Normal_3;
sampler2D Tile_NormalSampler_3 = sampler_state{
	Texture = (Tile_Normal_3);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//Secular Texture
texture Tile_Specular_0;
sampler2D Tile_SpecularSampler_0 = sampler_state{
	Texture = (Tile_Specular_0);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

texture Tile_Specular_1;
sampler2D Tile_SpecularSampler_1 = sampler_state{
	Texture = (Tile_Specular_1);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


texture Tile_Specular_2;
sampler2D Tile_SpecularSampler_2 = sampler_state{
	Texture = (Tile_Specular_2);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


texture Tile_Specular_3;
sampler2D Tile_SpecularSampler_3 = sampler_state{
	Texture = (Tile_Specular_3);
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
	float3 Binormal : BINORMAL;
	float3 Tangent : TANGENT;
	float2 BaseUV : TEXCOORD0;			//0~1 UV
	float2 TileUV : TEXCOORD1;			//Ÿ�� UV 

};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;	
	float2 BaseUV : TEXCOORD0;			//0~1 UV
	float2 TileUV : TEXCOORD1;			//Ÿ�� UV 
	float3 Normal : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
	float3 Tangent : TEXCOORD4;
	float3 WorldPos : TEXCOORD5;
	float4 ClipPos : TEXCOORD6;		//ȭ�� ���� �Ѱ�
};




//
// ���� ���̴� ��Ʈ�� �Լ�
//
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//�ͷ����� ������ġ�� �ٷ� �����̴�.
	float4 worldPos = Input.Position;

	//������ �� ���� ��ȯ
	Output.Position = mul(worldPos, matViewProjection);

	Output.BaseUV = Input.BaseUV;
	Output.TileUV = Input.TileUV;

	//TBN �Ѱ�
	Output.Normal = Input.Normal;
	Output.Binormal = Input.Binormal;
	Output.Tangent = Input.Tangent;

	//���� ��ġ �Ѱ� 
	Output.WorldPos = worldPos.xyz;


	//ȭ�� ����.
	Output.ClipPos = Output.Position;


	return Output;
}



//
// �ȼ� �Է� ����ü
//
struct PS_INPUT{
	float2 BaseUV : TEXCOORD0;			//0~1 UV
	float2 TileUV : TEXCOORD1;			//Ÿ�� UV 
	float3 Normal : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
	float3 Tangent : TEXCOORD4;
	float3 WorldPos : TEXCOORD5;
	float4 ClipPos : TEXCOORD6;			//ȭ�� ���� �Ѱ�
};



//��� �÷� ����
struct PS_OUTPUT{
	float4 stage0 : COLOR0;		//0 ���������� ��� �÷�
	float4 stage1 : COLOR1;			//1 ���������� ��� �÷�
	float4 stage2 : COLOR2;		//2 ���������� ��� �÷�
	float4 stage3 : COLOR3;		//3 ���������� ��� �÷�
};


//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
PS_OUTPUT ps_main(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;


	float4 splatColor = tex2D(Splat_Sampler, Input.BaseUV);

	//
	// Diffuse Tex
	//
	float4 diffuseTex0 = tex2D(Tile_Sampler_0, Input.TileUV);
	float4 diffuseTex1 = tex2D(Tile_Sampler_1, Input.TileUV);
	float4 diffuseTex2 = tex2D(Tile_Sampler_2, Input.TileUV);
	float4 diffuseTex3 = tex2D(Tile_Sampler_3, Input.TileUV);

	float black = 1.0f - saturate(splatColor.r + splatColor.g + splatColor.b);

	float4 diffuseColor =
		diffuseTex0 * splatColor.r +
		diffuseTex1 * splatColor.g +
		diffuseTex2 * splatColor.b +
		diffuseTex3 * black;

	//
	// Normal Tex
	//
	float4 normalTex0 = tex2D(Tile_NormalSampler_0, Input.TileUV);
	float4 normalTex1 = tex2D(Tile_NormalSampler_1, Input.TileUV);
	float4 normalTex2 = tex2D(Tile_NormalSampler_2, Input.TileUV);
	float4 normalTex3 = tex2D(Tile_NormalSampler_3, Input.TileUV);

	float3 normalTex =
		normalTex0.rgb * splatColor.r +
		normalTex1.rgb * splatColor.g +
		normalTex2.rgb * splatColor.b +
		normalTex3.rgb * black;

	//
	// specular Tex
	//
	float4 specularTex0 = tex2D(Tile_SpecularSampler_0, Input.TileUV);
	float4 specularTex1 = tex2D(Tile_SpecularSampler_1, Input.TileUV);
	float4 specularTex2 = tex2D(Tile_SpecularSampler_2, Input.TileUV);
	float4 specularTex3 = tex2D(Tile_SpecularSampler_3, Input.TileUV);

	float3 specularTex =
	specularTex0.rgb * splatColor.r +
	specularTex1.rgb * splatColor.g +
	specularTex2.rgb * splatColor.b +
	specularTex3.rgb * black;


	//�븻 Texture �� �÷��� ������ �´�.
	float3 norTexColor = normalTex;

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



	//depth ( �������� �������� )
	float depth = Input.ClipPos.z / Input.ClipPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	//float z = depth;
	//float a = 1000.0f / (1000.0f - 0.1f);
	//float b = -0.1f / (1000.0f - 0.1f);
	//depth = b / (z - a);

	Output.stage0 = float4(diffuseColor.rgb, 0);				//diffuse �÷�.
	Output.stage1 = float4(specularTex, 1);		//specular �÷�.
	Output.stage2 = float4(pX, pY, 0, 0);		//���� �� normal �� ����.
	Output.stage3 = float4(depth, 0, 0, 0);		//depth

	return Output;
}


float4 ps_shadow(PS_INPUT Input) : COLOR
{
	//depth ( �������� �������� )
	float depth = Input.ClipPos.z / Input.ClipPos.w;
	return float4(depth, 0, 0, 0);
}



//
// ��ũ�� ����
//
// Pass ������ �Ѵ�. 
technique Terrain
{
	pass p0
	{
		//���� ���̴� ����
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_main();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}
};

technique TerrainShadow
{
	pass p0
	{
		CULLMODE = NONE;
		//���� ���̴� ����
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

		//�ȼ� ���̴� ����
		PixelShader = compile ps_3_0 ps_shadow();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.

	}
};