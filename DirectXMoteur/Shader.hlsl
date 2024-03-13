Texture2D gDiffuseMap : register(t0);
SamplerState gsamPointWrap : register(s0);
//SamplerState gsamPointClamp : register(s1);
//SamplerState gsamLinearWrap : register(s2);
//SamplerState gsamLinearClamp : register(s3);
//SamplerState gsamAnisotropicWrap : register(s4);
//SamplerState gsamAnisotropicClamp : register(s5);


cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 Pos : POSITION;
    float2 Uv : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Uv : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
    // Just pass vertex color into the pixel shader.
    vout.Uv = vin.Uv;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gDiffuseMap.Sample(gsamPointWrap, pin.Uv);
   // return float4(1, 0, 0, 1);
}