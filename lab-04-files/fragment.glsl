varying vec4 position;
varying vec3 normal;
varying vec3 light_direction;

void main()
{
    vec4 ambient = vec4(1, 0, 0, 1);
    vec4 diffuse = vec4(0, 1, 0, 1);
    vec4 specular = vec4(0, 0, 1, 1);
    gl_FragColor = ambient + diffuse + specular;
}