
//
// ���̴� ���� ����
//

float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float4 mainColor;			//�� ���� ( �÷��� �������� float4 �� �޴´� )


//
// ���� �Է� ����ü ( �׸��� ��ü�� �������ִ� �������� )
//
struct VS_INPUT{
	float4 Position : POSITION;
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
	//Input.Position * matWorld = Output.Position;
	Output.Position = mul(Input.Position, matWorld);

	//������ �� ��ȯ
	Output.Position = mul(Output.Position, matView);

	//���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	Output.Color.x = Output.Position.x;
	Output.Color.y = Output.Position.y;
	Output.Color.z = 0.0f;
	Output.Color.w = 1.0f;

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