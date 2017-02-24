

//Diffuse Texture ���� ( �ܺ� ���� )
texture DiffuseTex;
sampler2D DiffuseSampler = sampler_state{
	Texture = DiffuseTex;		
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;

	ADDRESSU = CLAMP;
	ADDRESSV = CLAMP;
};

//Diffuse Light Texture ����
texture DiffuseLightTex;
sampler2D DiffuseLightSampler = sampler_state{
	Texture = DiffuseLightTex;		
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};


//Specular Texture ���� ( �ܺ� ���� )
texture SpecularTex;
sampler2D SpecularSampler = sampler_state{
	Texture = SpecularTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};


//Specular Light Texture ����
texture SpecularLightTex;
sampler2D SpecularLightSampler = sampler_state{
	Texture = SpecularLightTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};





float onePixelX = 0.0f;
float onePixelY = 0.0f;
float halfPixelX = 0.0f;
float halfPixelY = 0.0f;

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


VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//������������ġ�� �״�� ������ǥ�� ����.
	Output.Position = Input.Position;

	//�ؽ��� ��ǥ �ѱ��.
	//Output.Texcoord = Input.Texcoord;
	Output.Texcoord.x = Input.Texcoord.x + halfPixelX;
	Output.Texcoord.y = Input.Texcoord.y + halfPixelY;

	return Output;
}



//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
float4 ps_main(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);

	//Alpha ��  1.0 �ϼ��� ������ ���� �߻��Ѵ�.
	float emissive = ( diffuseColor.a );

	//ǥ�鿡�� �޴� Diffuse ������
	float3 diffuseLight = tex2D(DiffuseLightSampler, Texcoord).rgb;

	//ǥ�� Specular �ݻ緮
	float3 specularColor = tex2D(SpecularSampler, Texcoord).rgb;

	//Specular Color
	float3 specularLight = tex2D(SpecularLightSampler, Texcoord).rgb;

	float3 diff = lerp(diffuseLight, float3(1, 1, 1), emissive);

	float3 spec = lerp(specularLight, float3(0, 0, 0), emissive);
	//���� �÷�
	float3 finalColor = (diffuseColor * diff) + (specularColor * spec);

	return float4(finalColor, 1);
}


float ToonGyungGaeSu = 3.0f;

// ToonGyungGaeSu ��踦 ���� ����� �ش�.
float3 toonGyungGea(float3 val)
{
	//������ ��� �� val ���� ��� ���� ����.
	float interval = 1.0 / ToonGyungGaeSu;

	//floor �Ҽ��� �粸�ش�.
	float3 toon = floor(val / interval) * interval;

	return saturate( toon + interval );
}

// ToonGyungGaeSu ��踦 ���� ����� �ش�.
float3 toonGyungGeaSpec(float3 val)
{
	//������ ��� �� val ���� ��� ���� ����.
	float interval = 1.0 / 2.0f;

	//floor �Ҽ��� �粸�ش�.
	float3 toon = floor(val / interval) * interval;

	return toon;
}



//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
float4 ps_toon(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);

	//Alpha ��  1.0 �ϼ��� ������ ���� �߻��Ѵ�.
	float emissive = (diffuseColor.a);

	//ǥ�鿡�� �޴� Diffuse ������
	float3 diffuseLight = tex2D(DiffuseLightSampler, Texcoord).rgb;

	diffuseLight = toonGyungGea(diffuseLight);

	//ǥ�� Specular �ݻ緮
	float3 specularColor = tex2D(SpecularSampler, Texcoord).rgb;

	specularColor = toonGyungGeaSpec(specularColor);


	//Specular Color
	float3 specularLight = tex2D(SpecularLightSampler, Texcoord).rgb;

	float3 diff = lerp(diffuseLight, float3(1, 1, 1), emissive);

	float3 spec = lerp(specularLight, float3(0, 0, 0), emissive);

	//���� �÷�
	float3 finalColor = (diffuseColor * diff);

	return float4(finalColor, 1);
}


//
// �ܼ� ���
//
float4 ps_color(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);
	return diffuseColor;
}













//ȸ������...
float4 ps_gray(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);


	//�׷��̺����� ������ ����.
	//Red 0.299f
	//Green 0.587f
	//Blue 0.114f


	//ȸ������ �ٲٱ�...
	float grayScale =
		diffuseColor.r * 0.299f +
		diffuseColor.g * 0.587f +
		diffuseColor.b * 0.114f;

	return float4(grayScale, grayScale, grayScale, 1);
}


/////////////////////////////////////////////////////////////


//Additive Texture ���� ( �ܺ� ���� )
texture AdditiveTex;
sampler2D AdditiveSampler = sampler_state{
	Texture = AdditiveTex;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;

	ADDRESSU = CLAMP;
	ADDRESSV = CLAMP;
};

float addAmount = 1.0f;

float4 ps_addtive(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);
	float4 addColor = tex2D(AdditiveSampler, Texcoord) * addAmount;

	return diffuseColor + addColor;
}



/////////////////////////////////////////

float InBlack = 0.0f;		//�÷��� 0 ~ 255 �� ���� �̰� �̸��̸� 0 ���� ó���Ѵ�.
float Gamma = 1.0f;			//�÷��� 0 ~ 255 �� ���� �̰� �̻��̸� 1 ���� ó���Ѵ�.
float InWhite = 255.0f;		//���� 
float OutBlack = 0.0f;		//���� ó�� ���� ����� ���� 0 ~ 255 �� ���� �̰� �̸��̸� 0 ���� ó���Ѵ�.
float OutWhite = 255.0f;	//���� ó�� ���� ����� ���� 0 ~ 255 �� ���� �̰� �̻��̸� 1 ���� ó���Ѵ�.

float level(float color)
{
	// 0 ~ 1 ������ ���� �� �÷� ���� 0 ~ 255 ������...
	float c = color * 255.0f;

	//0 ������ �ȳ��� ����....
	c = max(0, c - InBlack);

	//�÷� �� InBlack ���� InWhite �� ���� ������ Factor �� 
	//�̰������� c �� 0 ~ 1 ������ ���� �ȴ�.
	c = saturate( c / (InWhite - InBlack));

	//���� ó�� 
	c = pow(c, Gamma);

	//�÷� Out ó�� 	( ���⼭ �ٽ� 0 ~ 255 �� �÷� ���� ���� �ȴ� )	
	//���⼭ ó���Ѵ� ���� 
	//���ݱ��� ���� c �� 0 ~ 1 ��������
	//outBlack �� outWhite �κ���.
	//c = c * (outWhite - outBlack) + outBlack;
	c = lerp(OutBlack, OutWhite, c);

	//���� �÷� 
	c = saturate(c / 255.0f);

	return c;
}

//ȸ������...
float4 ps_colorlevel(float2 Texcoord : TEXCOORD0) : COLOR
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);

	float r = level(diffuseColor.r);
	float g = level(diffuseColor.g);
	float b = level(diffuseColor.b);


	return float4(r, g, b, 1);
}




// Blur ����
/////////////////////////////////////////


float BlurWeights[13] =
{
	0.002216,		// -6
	0.008764,       // -5
	0.026995,       // -4
	0.064759,       // -3
	0.120985,       // -2
	0.176033,       // -1
	0.199471,		//Center
	0.176033,		//	1
	0.120985,		//	2
	0.064759,		//	3
	0.026995,		//	4
	0.008764,		//	5
	0.002216		//	6
};

float bulrPos[13] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	1,
	2,
	3,
	4,
	5,
	6
};


float blurScale = 1.0f;			//��������


float4 ps_BlurX(float2 Texcoord : TEXCOORD0) : COLOR0
{
	float4 finalColor = float4(0, 0, 0, 1);
	
	for (int i = 0; i < 13; i++)
	{
		float2 uv = Texcoord + float2(
			bulrPos[i] * onePixelX, 0) * blurScale;

		finalColor += tex2D(DiffuseSampler, uv) * BlurWeights[i];

	}

	return finalColor;
}



float4 ps_BlurY(float2 Texcoord : TEXCOORD0) : COLOR0
{
	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Texcoord + float2(
			0, bulrPos[i] * onePixelX) * blurScale;

		finalColor += tex2D(DiffuseSampler, uv) * BlurWeights[i];

	}

	return finalColor;
}

// OutLine
//////////////////////////////////////////////////////////


texture NormalTex;
sampler2D NormalSampler = sampler_state{
	Texture = NormalTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};



float3 GetWorldNormal(float2 uv)
{

	//����� �븻
	float2 pNor = tex2D(NormalSampler, uv).xy;	//X Y ���� ������ �´�.

		//����� �븻�� Ǯ�� ���� �븻�� ����
		float num = 2 / (1 + pNor.x * pNor.x + pNor.y * pNor.y);
	float x = pNor.x * num;
	float y = pNor.y * num;
	float z = num - 1;
	float3 worldNormal = float3(x, y, z);

	return worldNormal;

}



float mask[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1 };		//���ö� ����

float coordX[3] = { -1, 0, 1 };
float coordY[3] = { -1, 0, 1 };

float3 outlineColor = float3(0, 0, 0);

float4 ps_outline(float2 Texcoord : TEXCOORD0) : COLOR0
{

	float outlineStrength = 0.0f;

	//�ڽ��� ������ �ֺ� �ȼ����� 9�� 
	for (int i = 0; i < 9; i++)
	{
		float2 uv = float2(coordX[i % 3], coordY[i / 3]);
		uv.x *= onePixelX;
		uv.y *= onePixelY;
		uv = Texcoord + uv;


		float3 normalColor = GetWorldNormal(uv) * mask[i];

		outlineStrength += normalColor.x;
		outlineStrength += normalColor.y;
		outlineStrength += normalColor.z;
	}

	float value = saturate(abs(outlineStrength));
	value = pow(value, 2.0f);


	return float4(outlineColor, value);
}


// Fog
///////////////////////////////////////////////

texture DepthTex;
sampler2D DepthSampler = sampler_state{
	Texture = DepthTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};

float CamNear = 0.0f;
float CamFar = 0.0f;
float fogStart = 0.4f;	//���װ� ���۵Ǵ� depth
float fogEnd = 1.0f;	//�������� depth
float3 FogColor = float3(1, 1, 1);

float4 ps_AddFog(float2 Texcoord : TEXCOORD0) : COLOR0
{
	//�ܼ� ǥ�� �÷�...
	float4 diffuseColor = tex2D(DiffuseSampler, Texcoord);


	float depth = tex2D(DepthSampler, Texcoord).x;
	
	//���� depth ���� �������� ���ش�
	float z = depth;
	float a = CamFar / (CamFar - CamNear);
	float b = -CamNear / (CamFar - CamNear);
	depth = b / (z - a);



	float delta = fogEnd - fogStart;
	float fog = saturate(depth - fogStart) / delta;



	float3 finalColor = lerp(diffuseColor.rgb, FogColor, fog);

	return float4(finalColor, 1);
}




//////////////////////////////////////////////////////////

technique ComputeLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_main();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};

technique ComputeToonLight
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_toon();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};



technique GrayScale
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_gray();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};

technique ColorLevel
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_colorlevel();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};

technique BlurX
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_BlurX();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};


technique BlurY
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_BlurY();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};

technique AddFog
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_AddFog();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}


};


technique Outline
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_outline();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}

	
};

technique Additive
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_addtive();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}

	pass p1
	{
		ALPHABLENDENABLE = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;


		VertexShader = compile vs_3_0 vs_main();		//�̿���� �������̴� �Լ��� vs_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
		PixelShader = compile ps_3_0 ps_color();			//�̿���� �ȼ����̴� �Լ��� ps_main �̰� ���̴� ���� 3.0 ���� �����ڴ�.
	}
};
