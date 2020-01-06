function vec_distance(x0, y0, z0, x1, y1, z1)
    return math.sqrt(math.pow(x0 - x1, 2) + math.pow(y0 - y1, 2) + math.pow(z0 - z1, 2));
end

-- float vec3Length(struct Vec3* v){
-- //平方根x*x + y*y + z*z
-- return	(float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z );
-- }

function vec3Length(x, y, z)
    return math.sqrt(x * x + y * y + z * z);
end

-- void
-- vec3Normalize(struct Vec3* v){
-- float length = vec3Length(v);
-- if(length <=0){
-- 	printf("length:%.3f\n",length);
-- }
-- v->x = v->x / length;
-- v->y = v->y / length;
-- v->z = v->z / length;
-- }

function vec3Normalize(x, y, z)
    local len = vec3Length(x, y, z);
    if (length <= 0) then
        func_error("vec3Normalize!!!");
    end
    x = x / length;
    y = y / length;

    z = z / length;
    return x, y, z;
end

-- 叉乘
function vec3_cross(x0, y0, z0, x1, y1, z1)
    return y0 * z1 - z0 * y1, z0 * x1 - x0 * z1, x0 * y1 - y0 * x1;
end

-- 点乘
function vec3_dot(x0, y0, z0, x1, y1, z1)
    return x0 * x1 + y0 * y1 + z0 * z1;
end