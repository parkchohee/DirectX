
//
// ���̴� ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float3 lightDir;			//���� ���� ����

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
	Output.Position = mul(Input.Position, matWorld);

	//������ �� ��ȯ
	Output.Position = mul(Output.Position, matView);

	//���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//�Էµ� �븻 ���� ���� ������ ��������.
	float3 worldNormal = mul(Input.Normal, (float3x3)matWorld);
	//�������� �Ծ������� �𸣴ϱ� ����ȭ
	worldNormal = normalize( worldNormal );


	//����Ʈ ���� �������� �������� ������.
	float diff = dot(worldNormal, -lightDir);
	if (diff < 0.0f)
		diff = 0.0f;



	//�÷��� �븻 ���Ͱ��� �Ẹ��.
	//Output.Color.x = Input.Normal.x;
	//Output.Color.y = Input.Normal.y;
	//Output.Color.z = Input.Normal.z;

	//���͸� ���� ����� �̸��� ������ ���� �����ִ�.
	//x == r, y == g, b == z, w == a
	//Output.Color.r = worldNormal.r;
	//Output.Color.g = worldNormal.g;
	//Output.Color.b = worldNormal.b;
	//Output.Color.a = 1.0f;

	//Output.Color.rgb = worldNormal;
	//Output.Color.xyz = worldNormal;
	
	//Output.Color.r = diff;
	//Output.Color.g = diff;
	//Output.Color.b = diff;
	//Output.Color.rgb = diff.rrr;  //��Į���� �����ϰ� x ������ ��� vector3 ��ҷ� ���ڴ�.
	
	Output.Color.rgb = mainColor.rgb * diff;
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