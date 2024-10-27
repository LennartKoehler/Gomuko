

struct Action{
	int position_x;
	int position_y;
	int value;
	Action(int x, int y, int v) : position_x(x), position_y(y), value(v){}
	std::string to_const_char(){
		std::stringstream ss;
        ss << "Position X: " << position_x << ", Position Y: " << position_y << ", Value: " << value;
        return ss.str();
	}
};