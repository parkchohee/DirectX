
//
// 셰이더 전역 변수
//

float4x4	matWorld;				//월드 행렬
float4x4	matViewProjection;		//뷰 행렬

float3 viewPos;					//뷰 위치

//반사될 환경 큐브 맵
textureCUBE Environment_Tex;
samplerCUBE EnvironmentSampler = sampler_state
{
	Texture = (Environment_Tex);
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MAGFILTER = LINEAR;
};


//
// 정점 입력 구조체 ( 그리는 물체가 가지고있는 정점정보 )
//
struct VS_INPUT{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
};

//
// 정점 출력 구조체
//
struct VS_OUTPUT{
	float4 Position : POSITION;		//동차 좌표까지 변환이 완료된 정점의 위치
	float3 Normal : TEXCOORD0;		//이건 픽셀셰이더로 전달된다.
	float3 ViewDir : TEXCOORD1;		//뷰의 방향
};

//
// 정점 셰이더 엔트리 함수
//
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//정점의 월드 변환
	float4 worldPos = mul(Input.Position, matWorld);

	//정점의 뷰 투영변환
	Output.Position = mul(worldPos, matViewProjection);

	//노말 월드 변환.
	Output.Normal = mul(Input.Normal, (float3x3)matWorld);

	//시야 방향
	Output.ViewDir = viewPos - worldPos.xyz;


	return Output;

}


//
// 픽셀 입력 구조체
//
struct PS_INPUT{
	float3 Normal : TEXCOORD0;		//이건 픽셀셰이더로 전달된다.
	float3 ViewDir : TEXCOORD1;		//뷰의 방향
};


//
// 픽셀 셰이더 엔트리 함수
//
float4 ps_main(PS_INPUT Input) : COLOR
{
	float3 worldNormal = normalize(Input.Normal);
	float3 viewDir = normalize(Input.ViewDir);

	//들어오는 View의 방향
	float3 InDirView = -viewDir;

	//들어오는 View 방향을 월드 노말에 반사
	float3 reflectView = reflect(InDirView, worldNormal);


	//Cube 컬러 샘플링
	float4 finalColor = texCUBE(EnvironmentSampler, reflectView);
	
	return finalColor;
}

//
// 테크닉 설정
//
// Pass 설정을 한다. 
technique ColorShader
{


	//Pass 설정
	//해당 Pass 의 정점 셰이더를 셰이더 얼마버전으로 돌릴건지 또한 엔트리함수는 어떻게 되는지 결정
	//그리고 픽셀 셰이더를 셰이더 얼마버전으로 돌릴건지 또한 엔트리함수는 어떻게 되는지 결정
	pass p0
	{

		//정점 셰이더 설정
		VertexShader = compile vs_3_0 vs_main();		//이예기는 정점셰이더 함수는 vs_main 이고 셰이더 버전 3.0 으로 돌리겠다.

		//픽셀 셰이더 설정
		PixelShader = compile ps_3_0 ps_main();			//이예기는 픽셀셰이더 함수는 ps_main 이고 셰이더 버전 3.0 으로 돌리겠다.

	}


};