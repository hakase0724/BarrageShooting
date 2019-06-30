 struct PerInstanceData
{
    matrix instanceMat;
    float4 instanceColor;
};

StructuredBuffer<PerInstanceData> mInstanceData : register(t0);;
Texture2D mTexture : register(t1); //�e�N�X�`���[
SamplerState mSampler : register(s0); //�T���v���[

// ���_�V�F�[�_����o�͂����\����
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TextureUV : TEXCOORD;
    uint id : ID;
};
 
// ���_�V�F�[�_
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD,uint instanceID : SV_InstanceID)
{
    VS_OUTPUT output;
    output.Pos = mul(Pos, mInstanceData[instanceID].instanceMat);
    output.TextureUV = Tex;
    output.id = instanceID;
    return output;
}
 
// �s�N�Z���V�F�[�_
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 tex = mTexture.Sample(mSampler, input.TextureUV);
    if (tex.a <= 0) discard;
    tex *= mInstanceData[input.id].instanceColor;
    return tex;
}