
//
// ���̴� ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float3 lightDir;			//���� ���� ����
float3 camPos;				//������ ��ġ

float4 mainColor = float4(1, 1, 0, 1); // default �� �����



//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT{
	float4 Position : POSITION;		//�����ǰ��� ������ float4 �޴´�.
	float3 Normal : NORMAL;
};

//
// ���� ��� ����ü
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//���� ��ǥ���� ��ȯ�� �Ϸ�� ������ ��ġ
	float4 Color : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
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
	//�������� �Ծ������� �𸣴ϱ� ����ȭ
	worldNormal = normalize( worldNormal );

	//����Ʈ ���� �������� �������� ������.
	float diff = dot(worldNormal, -lightDir);
	diff = saturate(diff);		//���� 0 ~ 1 �� ©���ش�.

	//diffColor ���Ϸ�
	float3 diffColor = mainColor.rgb * diff;
	


	//relflect
	float3 refl = reflect( lightDir, worldNormal );		//������ ������ worldNormal �� �ݻ�� �ݻ� ���͸� ��´�.

	//�� ����
	float3 viewDir = camPos.xyz - worldPos.xyz;
	viewDir = normalize( viewDir);

	float spec = dot(viewDir, refl);
	spec = saturate(spec);

	//spec 30 �� �ϰڴ�.
	spec = pow(spec, 30);		//�̰��� Ŀ������ ���̶���Ʈ �κ� �۾����� �� ��ī�ο�����.


	float3 specColor = float3(1, 1, 1) * spec;


	Output.Color.rgb = diffColor + specColor;
	Output.Color.a = 1.0f;


	return Output;
}


//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(float4 Color : TEXCOORD0) : COLOR
{
	return Color;
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