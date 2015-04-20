static const int MAX_MATRICES = 32;

float4x4 g_worldViewMat;
float4x4 g_projMat;

texture  g_txCubeMap;
texture faceTex;


samplerCUBE g_envCubeTexture = 
sampler_state
{
    Texture = <g_txCubeMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

sampler2D textureSampler = sampler_state {
    Texture = (faceTex);
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct vs_in_t
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex_coord : TEXCOORD0;
};

struct vs_out_t
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD0;
    float3 envTex : TEXCOORD1;

};


vs_out_t mirror_VS(vs_in_t vs_in)
{
    vs_out_t vs_out = (vs_out_t)0;

    vs_out.tex = vs_in.tex_coord;

    vs_out.pos = mul(vs_in.pos, g_worldViewMat);

    //
    // Compute normal in camera space
    //
    float3 vN = mul(vs_in.normal, g_worldViewMat);
    vN = normalize(vN);

    //
    // Obtain the reverse eye vector
    //
    float3 vEyeR = -normalize(vs_out.pos);

        //
        // Compute the reflection vector
        //
        float3 vRef = 2 * dot(vEyeR, vN) * vN - vEyeR;

        //
        // Store the reflection vector in texcoord0
        //
        vs_out.envTex = vRef;

    //
    // Apply the projection
    //
    vs_out.pos = mul(vs_out.pos, g_projMat);

    return vs_out;
}

float4 mirror_PS(vs_out_t ps_in) : COLOR0
{
    //return 1.0 * texCUBE(g_envCubeTexture, ps_in.envTex);
    return texCUBE(g_envCubeTexture, ps_in.envTex);
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