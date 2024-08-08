#version 330 core
out vec4 FragColor;

in vec2 TextCord;
uniform sampler2D textureWall;
uniform sampler2D textureLe;
uniform float ratio;

void main()
{
    FragColor = mix(texture(textureWall,TextCord),texture(textureLe,TextCord),ratio);
   // FragColor = texture(textureLe,TextCord);
}
