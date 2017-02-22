//
// ���̴� ���� ����
//
float4x4	matWorld;				//���� ���
float4x4	matViewProjection;		//�� * �������� ���
float4x4	matInvViewProjection;	//�� * �������� �����

//���⼺ ������ ���� ����
float3 lightDir = float3(1, 0, 0);

//�������� ���� ��ġ
float3 lightPos = float3(0, 0, 0);

//�������� �����
float maxRange = 10;
float minRange = 1;
float fallOff = 1;


//������ ����
float3 lightColor = float3(1, 1, 1);

//������ ����
float intensity = 0.0f;

//�������� ��ġ
float3 camPos = float3(0, 0, 0);


float halfPixelSizeX;
float halfPixelSizeY;



//WorldNormal
texture NormalTex;
sampler2D NormalSampler = sampler_state{

	Texture = NormalTex;		
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};

//WorldPosition
texture DepthTex;
sampler2D DepthSampler = sampler_state{
	Texture = DepthTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
};


//ShadowTex
texture ShadowTex;
sampler2D ShadowSampler = sampler_state{
	Texture = ShadowTex;
	MIPFILTER = POINT;
	MAGFILTER = POINT;
	MINFILTER = POINT;
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;



};

//DirectionLightViewProjection
float4x4 matDirectionLightViewProj;

float ambient = 0.2f;


struct VS_INPUT{
	float4 Position : POSITION;
};

struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float4 ClipPosition : TEXCOORD0;	//�ʼ����̴��� ���޵Ǵ� ���� ��ǥ
};

struct PS_OUTPUT{
	float4 diffuseLight : COLOR0;
	float4 specularLight : COLOR1;
};


//
// Direction Light
//


//���⼺������ ���ؽ� ���̴�
VS_OUTPUT vs_Direction(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//������������ġ�� �״�� ������ǥ�� ����.
	Output.Position = Input.Position;
	Output.ClipPosition = Output.Position;	//������ PixelShader �� �ѱ��.

	return Output;
}

//���⼺������ �ȼ� ���̴�
PS_OUTPUT ps_Direction(float4 ClipPosition : TEXCOORD0)
{
	float2 clip = ClipPosition.xy / ClipPosition.w;
	float2 uv = clip;


	//-1 ~ 1 ������ 0 ~ 1 ������..
	//uv.x = (uv.x + 1.0f) * 0.5f;
	//uv.y = (uv.y + 1.0f) * 0.5f;
	uv = (uv + 1.0f) * 0.5f;		//vector �� ��Į�� ���� �������� ( ��Ҹ� �ٰ���� )
	uv.y = 1.0f - uv.y;				//Y �� ����...

	//�� �ȼ� �о�
	uv.x += halfPixelSizeX;
	uv.y += halfPixelSizeY;

	//���� �븻
	//float3 worldNormal = tex2D(NormalSampler, uv).xyz;	//�븻 �������

	//����� �븻
	float2 pNor = tex2D(NormalSampler, uv).xy;	//X Y ���� ������ �´�.
	
	//����� �븻�� Ǯ�� ���� �븻�� ����
	float num = 2 / (1 + pNor.x * pNor.x + pNor.y * pNor.y);
	float x = pNor.x * num;
	float y = pNor.y * num;
	float z = num - 1;
	float3 worldNormal = float3(x, y, z);


	//������ ��´�.
	float depth = tex2D(DepthSampler, uv).r;
	//���� ��ǥ
	float4 clipPos = float4(clip.x, clip.y, depth, 1);
	//���� ��ġ
	float4 worldPos = mul(clipPos, matInvViewProjection);
	worldPos.xyz /= worldPos.w;


	//����Ʈ ViewProjection �� ���� ��ǥ
	float4 lightCamClip = mul(float4(worldPos.xyz, 1), 
		matDirectionLightViewProj);
	//ShadowMap �� UV �� ��´�.
	float2 shadowUV = (lightCamClip.xy + 1.0f) * 0.5f;
	//y ����
	shadowUV.y = 1.0f - shadowUV.y;

	//������ �ʿ� ���� ���� 
	float shadowDepth = tex2D(ShadowSampler, shadowUV);
	float myDepthToLight = lightCamClip.z;



	//diffuse �� ���
	float d = dot(worldNormal, -lightDir);
	float diff = d;

	float shadowBias = 0.002f;
	//�׸��� ������. ���� ���� ����.
	if (shadowDepth + shadowBias < myDepthToLight && 
		lightCamClip.z <= 1.0f ){
		diff = 0.0f;
	}
	//Ambient ����
	diff = max(ambient, diff);


	//float amount = diff * intensity * (1.0f - shadowIntensity );
	
	float amount = diff * intensity;
	float3 diffuseColor = lightColor * amount;


	//
	// ���ݻ� ���
	//
	//�ش� �ȼ���ġ�� ������ġ
	//��ġ�������� �����ڹ��⺤��
	float3 viewDir = camPos - worldPos.xyz;
	viewDir = normalize(viewDir);

	//������ �븻�� ���ݻ�Ǿ� ������ ����.
	//float3 reflectLight = reflect(lightDir, worldNormal); 
	float3 reflectLight = lightDir + d * worldNormal * 2.0f;
	reflectLight = normalize(reflectLight); 

	//���ݻ緮
	float spec = dot(reflectLight, viewDir);
	spec = pow(spec, 20) * amount;

	//���� ���ݻ� �÷�....
	float3 specularColor = float3(spec, spec, spec);

	PS_OUTPUT Output;
	Output.diffuseLight = float4(diffuseColor, 1);
	Output.specularLight = float4(specularColor, 1);

	return Output;
}



//
// Point Light
//


//�� ������ ���ؽ� ���̴�
VS_OUTPUT vs_Point(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//���� ��ġ�� ȭ�� ������ �������Ѵ�.
	Output.Position = mul(Input.Position, matWorld);
	Output.Position = mul(Output.Position, matViewProjection);
	
	Output.ClipPosition = Output.Position;	//������ PixelShader �� �ѱ��.

	return Output;
}

//�������� �ȼ� ���̴�
PS_OUTPUT ps_Point(float4 ClipPosition : TEXCOORD0)
{
	float2 clip = ClipPosition.xy / ClipPosition.w;
	float2 uv = clip;

	//-1 ~ 1 ������ 0 ~ 1 ������..
	//uv.x = (uv.x + 1.0f) * 0.5f;
	//uv.y = (uv.y + 1.0f) * 0.5f;
	uv = (uv + 1.0f) * 0.5f;		//vector �� ��Į�� ���� �������� ( ��Ҹ� �ٰ���� )
	uv.y = 1.0f - uv.y;				//Y �� ����...

	//�� �ȼ� �о�
	uv.x += halfPixelSizeX;
	uv.y += halfPixelSizeY;




	//������ ��´�.
	float depth = tex2D(DepthSampler, uv).r;
	//���� ��ǥ
	float4 clipPos = float4(clip.x, clip.y, depth, 1);
	//���� ��ġ
	float4 worldPos = mul(clipPos, matInvViewProjection);
	worldPos.xyz /= worldPos.w;

	//���� �븻
	//float3 worldNormal = tex2D(NormalSampler, uv).xyz;	//�븻 �������

	//����� �븻
	float2 pNor = tex2D(NormalSampler, uv).xy;	//X Y ���� ������ �´�.

	//����� �븻�� Ǯ�� ���� �븻�� ����
	float num = 2 / (1 + pNor.x * pNor.x + pNor.y * pNor.y);
	float x = pNor.x * num;
	float y = pNor.y * num;
	float z = num - 1;
	float3 worldNormal = float3(x, y, z);



	//���� ����
	float3 toLight = lightPos - worldPos.xyz;
	
	//�������� �Ÿ�
	float distToLight = length(toLight);	//���� ����
	//toLight = normalize(toLight);
	toLight *= 1.0f / distToLight;			//����ȭ



	//Max �� Min �� ����
	float delta = maxRange - minRange;		
	float t = saturate( (distToLight - minRange) / delta );
	t = 1.0f - t;

	//fallOff ����
	t = pow(t, fallOff);


	//diffuse �� ���
	float d = dot(worldNormal, toLight);
	float diff = saturate(d);

	float amount = diff * intensity * t;

	float3 diffuseLight = lightColor * amount;


	//
	// ���ݻ� ���
	//

	//��ġ�������� �����ڹ��⺤��
	float3 viewDir = camPos - worldPos.xyz;
	viewDir = normalize(viewDir);

	//������ �븻�� ���ݻ�Ǿ� ������ ����.
	//float3 reflectLight = reflect(lightDir, worldNormal); 
	float3 reflectLight = -toLight + d * worldNormal * 2.0f;
	reflectLight = normalize(reflectLight);

	//���ݻ緮
	float spec = dot(reflectLight, viewDir);
	spec = pow(spec, 20) * amount;

	//���� ���ݻ� �÷�....
	float3 specularColor = float3(spec, spec, spec);

	PS_OUTPUT Output;
	Output.diffuseLight = float4(diffuseLight, amount);
	Output.specularLight = float4(specularColor, amount);

	return Output;
}


//���⼺ ���� technique
technique DirectionLight
{
	pass p0
	{
		ZWRITEENABLE = FALSE;		//����� ���̸� ������...
		VertexShader = compile vs_3_0 vs_Direction();
		PixelShader = compile ps_3_0 ps_Direction();
	}
};


//�� ���� 
technique PointLight
{
	//���ٽ��� ���� ���� Pass
	pass StencilPass
	{
		ZWRITEENABLE = FALSE;		//����� ���̸� ������...
		CULLMODE = NONE;
		COLORWRITEENABLE = 0x0;
		ZENABLE = TRUE;			//���� �׽�Ʈ �ݵ�� �Ѵ�

		STENCILENABLE = TRUE;	//���ٽ� ������� Ȱ��ȭ
		STENCILFUNC = ALWAYS;	//�������Ǵ� �������� ���ٽ��� �׻�����ȴ�.
		STENCILPASS = KEEP;
		STENCILFAIL = KEEP;
		STENCILZFAIL = INCR;	//������

		VertexShader = compile vs_3_0 vs_Point();
		PixelShader = NULL;			//������ �÷��ȱ׸�....
	}

	pass RenderPass
	{
		ZWRITEENABLE = FALSE;		//����� ���̸� ������...

		COLORWRITEENABLE = 0xFFFFFFFF;
		ZENABLE = FALSE;
		CULLMODE = CW;		//�̰� CW �� �ؾ��� NONE ���� �ϸ� ���� �ȼ��� 2 �������� �ɼ� ����
		
		STENCILREF = 0x01;
		STENCILFUNC = LESSEQUAL;	//�������Ǵ� �������� ���ٽ��� �׻�����ȴ�.
		STENCILPASS = KEEP;
		STENCILFAIL = KEEP;
		STENCILZFAIL = KEEP;	//������


		//Alpha ���� ����
		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = ONE;

		VertexShader = compile vs_3_0 vs_Point();
		PixelShader = compile ps_3_0 ps_Point();
	}
};







