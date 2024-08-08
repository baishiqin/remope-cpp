#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTextCord;

out vec3 ourColor;
out vec2 TextCord;
uniform mat4 RotationMatrix;//旋转
void main()
{
    gl_Position = RotationMatrix* vec4(aPos.x,aPos.y,aPos.z,1.0f);
    ourColor = aColor;
    TextCord = aTextCord;
    //TextCord = vec2(1-aTextCord.x,aTextCord.t);
}
