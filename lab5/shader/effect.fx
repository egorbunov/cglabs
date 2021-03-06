static const int MAX_MATRICES = 32;

float4x4 g_worldViewMat;
float4x4 g_projMat;
float g_fresPow;
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

sampler2D textureSampler =
sampler_state {
    Texture = (faceTex);
    MinFilter = Point;
    MagFilter = Point;
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
    float coef : TEXCOORD2;
};


vs_out_t mirror_VS(vs_in_t vs_in)
{
    vs_out_t vs_out = (vs_out_t)0;

    vs_out.tex = vs_in.tex_coord; // object real tex coord
    vs_out.pos = mul(vs_in.pos, g_worldViewMat);
    float3 vN =  mul(vs_in.normal, (float3x3) g_worldViewMat); // normal in camera space
    vN = normalize(vN);
    float3 vEyeR = (float3) -normalize(vs_out.pos); // the reverse eye vector
    float3 vRef = 2 * dot(vEyeR, vN) * vN - vEyeR; // reflection
    vs_out.envTex = vRef; // save in tex coord
    vs_out.pos = mul(vs_out.pos, g_projMat); // applying projection

    // calculating Fresnel coef approx.
    float angle_cos = abs(dot(-vEyeR, vN));
    vs_out.coef.x = 1 / pow((1 + angle_cos), g_fresPow);

    return vs_out;
}

float4 mirror_PS(vs_out_t ps_in) : COLOR0
{
    float4 tc = texCUBE(g_envCubeTexture, ps_in.envTex); // cube tex
    float4 tr = tex2D(textureSampler, ps_in.tex); // real obj. tex

    return tr * (1 - ps_in.coef.x) + tc * (ps_in.coef.x);
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