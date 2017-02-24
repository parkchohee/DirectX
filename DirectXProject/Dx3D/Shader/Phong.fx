
//
// ���̴� ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float3 lightDir;			//���� ���� ����
float3 camPos;				//������ ��ġ

//ǥ���� ����
float4 diffuse = float4(1, 1, 1, 1); // default �� �����
float3 specular = float3(1, 1, 1);  //ǥ���� ���ݻ� ��
float specPower = 30;				//���ݻ��� ��ī�ο�

//���� ����
float3 lightColor = float3(1, 1, 1);

//ȯ�汤�� ����
float3 ambient = float3(0.5f, 0.1f, 0.1f);




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
	float3 Normal : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
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

	//���� �븻 �ѱ��.
	Output.Normal = worldNormal;

	//�� ���� �ѱ��.
	Output.ViewDir = camPos - worldPos.xyz;


	return Output;
}


//�ȼ����̴� �Է� ����ü
struct PS_INPUT{
	float3 Normal : TEXCOORD0;		//�̰� �ȼ����̴��� ���޵ȴ�.
	float3 ViewDir : TEXCOORD1;		//�̰� �ȼ����̴��� ���޵ȴ�.
};



//
// �ȼ� ���̴� ��Ʈ�� �Լ�
//
// �ʼ����̴��� �ǹ��� ���������� ��µ� ������ ��� ��µǴ����� ����Ͽ�
// ��µ� ������ �����Ѵ�.
float4 ps_main(PS_INPUT Input) : COLOR
{
	//���� �븻 ����ȭ
	float3 worldNormal = normalize(Input.Normal);

	//�þ� ���� ����ȭ
	float3 viewDir = normalize(Input.ViewDir);


	//
	// Diffuse Ȯ�걤 ���
	//

	//������ �ٿ����� ���� �븻�� ����
	float d = dot(worldNormal, -lightDir);

	//0~1 ������ ������ ©��
	float diff = saturate(d);
	diff = max(diff, 0.3f);


	//�߻�Ǵ� ���� Ȯ�걤�� ����
	float3 diffColor = diffuse.rgb * lightColor * diff;

	//
	// Specular ���ݻ籤 ���
	//

	//������ ���� �븻�� ���ݻ�� ���⺤��
	//float3 refl = reflect( lightDir, worldNormal );
	//float3 proj = dot(-lightDir, worldNormal) * worldNormal;
	//float3 proj = d * worldNormal;
	//float3 refl = lightDir + proj * 2.0f;
	float3 refl = lightDir + ( d * worldNormal * 2.0f);
	refl = normalize( refl );

	//���ݻ� ��
	float spec = dot(refl, viewDir);
	spec = saturate(spec);

	//Power ����
	spec = pow(spec, specPower);

	//�߻�Ǵ� ���� ���ݻ��� ����
	//float3 specColor = (specular * lightColor) * spec;			//���ݻ簡 Ȯ�걤�� ������ �ȹ޴°��
	float3 specColor = (specular * lightColor) * spec * diff;		//���ݻ簡 Ȯ�걤�� ������ �޴°��
	

	//���� ������ ����
	//float3 finalColor = diffColor + specColor + ambient;
	float3 finalColor = diffColor + specColor;

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