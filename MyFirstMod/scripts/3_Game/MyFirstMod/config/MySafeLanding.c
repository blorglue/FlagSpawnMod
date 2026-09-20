class MySafeLanding
{
	static const float MIN_RADIUS = 0.8;    // keep clear of the pole itself
	static const float MAX_RADIUS = 1.5;    // pile is ~3 m wide, so 1.5 m keeps you on the rocks
	static const float DROP_HEIGHT = 1.0;   // spawn slightly above the pile and settle onto it

	// Random point on the flag's rock pile. `center` is the flag's position.
	static vector Find(vector center, float radius = MAX_RADIUS)
	{
		float angle = Math.RandomFloatInclusive(0, 6.283185);
		float dist = Math.RandomFloatInclusive(MIN_RADIUS, radius);
		float x = center[0] + dist * Math.Cos(angle);
		float z = center[2] + dist * Math.Sin(angle);

		// never lower than the terrain, in case center's height isn't set
		float y = Math.Max(center[1], GetGame().SurfaceY(x, z));

		return Vector(x, y + DROP_HEIGHT, z);
	}
}