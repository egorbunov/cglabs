static const int MAX_MATRICES = 32;

float4x4 g_worldViewMat;
float4x4 g_projMat;

texture  g_txCubeMap;

samplerCUBE g_envCubeTexture = 
sampler_state
{
    Texture = <g_txCubeMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

void mirror_VS(float4 Pos : POSITION,
               float3 Normal : NORMAL,
               out float4 oPos : POSITION,
               out float3 EnvTex : TEXCOORD0)
{
    oPos = mul(Pos, g_worldViewMat);

    //
    // Compute normal in camera space
    //
    float3 vN = mul(Normal, g_worldViewMat);
    vN = normalize(vN);

    //
    // Obtain the reverse eye vector
    //
    float3 vEyeR = -normalize(oPos);

        //
        // Compute the reflection vector
        //
        float3 vRef = 2 * dot(vEyeR, vN) * vN - vEyeR;

        //
        // Store the reflection vector in texcoord0
        //
        EnvTex = vRef;

    //
    // Apply the projection
    //
    oPos = mul(oPos, g_projMat);
}

float4 mirror_PS(float3 Tex : TEXCOORD0) : COLOR
{
    return 1.0 * texCUBE(g_envCubeTexture, Tex);
}

technique mirror_technique
{
    pass P0
    {
        Lighting = false;
        VertexShader = compile vs_2_0 mirror_VS();
        PixelShader  = compile ps_2_0 mirror_PS();
    }
}