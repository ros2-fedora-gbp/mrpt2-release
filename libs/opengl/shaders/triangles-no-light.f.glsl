R"XXX(#version 300 es

// FRAGMENT SHADER: Default shader for MRPT CRenderizable objects
// Jose Luis Blanco Claraco (C) 2019-2022
// Part of the MRPT project

in mediump vec4 frag_materialColor;
out mediump vec4 color;

void main()
{
    color = frag_materialColor;
}
)XXX"