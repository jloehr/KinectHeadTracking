struct CameraConstantBuffer
{
	matrix View;
	matrix Projection;
};

ConstantBuffer<CameraConstantBuffer> Camera : register(b0);

struct ObjectConstantBuffer
{
	matrix World;
};

ConstantBuffer<ObjectConstantBuffer> Object : register(b1);

struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR0;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
};

PSInput VShader(VSInput Input)
{
	PSInput Output;

	Output.Color = float4(Input.Color, 1.0f);

	Output.Position = float4(Input.Position, 1.f);
	Output.Position = mul(Output.Position, Object.World);
	Output.Position = mul(Output.Position, Camera.View);
	Output.Position = mul(Output.Position, Camera.Projection);

	return Output;
}

float4 PShader(PSInput Input) : SV_TARGET
{
	return Input.Color;
}

