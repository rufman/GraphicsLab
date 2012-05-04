Vec3 = {}
function Vec3:Subtract(a, b)
	return {a[1] - b[1], a[2] - b[2], a[3] - b[3]}
end

function Vec3:Normalize(vec)
	len = math.sqrt(vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3])		
	v = {vec[1] / len, vec[2] / len, vec[3] / len}
	
	return v
end

function Vec3:Cross(a, b)
	return {a[2] * b[3] - a[3] * b[2], a[3] * b[1] - a[1] * b[3], a[1] * b[2] - a[2] * b[1]}
end

function Vec3:Dot(a, b)
	return (a[1] * b[1] + a[2] * b[2] + a[3] * b[3])
end

function Vec3:Length(vec)
	len = math.sqrt(vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3])		
	
	return len
end

function Vec3:Distance(a, b)
	vec_diff	= Vec3:Subtract( a, b )
	
	distance	= Vec3:Length( vec_diff )
	
	return distance
end
