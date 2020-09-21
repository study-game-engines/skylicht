struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex0 : TEXCOORD0;
	float4 worldPos: WORLD_POSITION;
};
cbuffer cbPerFrame
{
	float4 uNoiseOffset;
	float4 uElectricColor;
};
float hash(float3 p)
{
    p = frac( p*0.3183099+.1 );
	p *= 17.0;
    return frac( p.x*p.y*p.z*(p.x+p.y+p.z) );
}
float noise( in float3 x )
{
    float3 i = floor(x);
    float3 f = frac(x);
    f = f*f*(3.0-2.0*f);
    return lerp(lerp(lerp( hash(i+float3(0,0,0)),
                        hash(i+float3(1,0,0)),f.x),
                   lerp( hash(i+float3(0,1,0)),
                        hash(i+float3(1,1,0)),f.x),f.y),
               lerp(lerp( hash(i+float3(0,0,1)),
                        hash(i+float3(1,0,1)),f.x),
                   lerp( hash(i+float3(0,1,1)),
                        hash(i+float3(1,1,1)),f.x),f.y),f.z);
}
static const float3x3 m = float3x3( 0.00, 0.80, 0.60,
                    -0.80, 0.36, -0.48,
                    -0.60, -0.48, 0.64 );
float pnoise( float3 q )
{
	float f = 0.5000*noise( q );
	q = mul(q, m)*2.01;
	f += 0.2500*noise( q );
	q = mul(q, m)*2.02;
	f += 0.1250*noise( q );
	q = mul(q, m)*2.03;
	f += 0.0625*noise( q );
	q = mul(q, m)*2.01;
	return f;
}
float fbm(float3 p)
{
	float z=2.;
	float rz = 0.;
	rz+= abs((noise(p)-0.5)*2.)/z;
	z = z*2.;
	p = p*2.;
	rz+= abs((noise(p)-0.5)*2.)/z;
	z = z*2.;
	p = p*2.;
	rz+= abs((noise(p)-0.5)*2.)/z;
	z = z*2.;
	p = p*2.;
	rz+= abs((noise(p)-0.5)*2.)/z;
	z = z*2.;
	p = p*2.;
	rz+= abs((noise(p)-0.5)*2.)/z;
	z = z*2.;
	p = p*2.;
	return rz;
}
float4 main(PS_INPUT input) : SV_TARGET
{
	float rz = fbm(uNoiseOffset.xyz + input.worldPos.xyz * uNoiseOffset.w);
	rz *= 2.0f;
	float3 col = uElectricColor.rgb / rz;
	return input.color * float4(col, 1.0);
}