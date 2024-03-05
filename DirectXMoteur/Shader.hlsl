/*cbuffer cbEntity : register(b0)
{
    float4x4 gEntityProj;
}*/

cbuffer cbWord : register(b0)
{
    float4x4 gWorldProj;
}

struct VertexIn
{
    float3 posInt : POSITION;
    float4 colorInt : COLOR;
};

struct VertexOut
{
    float4 posOut : SV_Position;
    float4 colorOut : COLOR0;
};

VertexOut VS(VertexIn _vertexIn)
{
    VertexOut vertexOut;
    
    vertexOut.posOut = mul(float4(_vertexIn.posInt, 1.0f), gWorldProj);
    
    
    vertexOut.colorOut = _vertexIn.colorInt;
    
    return vertexOut;
}

float4 PS(VertexOut _vertexOut) : SV_Target
{
    return _vertexOut.colorOut;
}
