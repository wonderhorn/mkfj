#include<string>
#include<list>
namespace space_subwindow
{
	class GRAPH
	{
	public:
		virtual void paint(HDC hDC) = 0;
	};

	class LETTERBOX: public GRAPH
	{
	public:
		LETTERBOX(const char* str, int x, int y);
		void paint(HDC hDC);
	private:
		std::string str;
		int x, y;
	};

	class LINE: public GRAPH
	{
	public:
		LINE(int orix, int oriy, int terx, int tery);
		void paint(HDC hDC);
	private:
		int orix, oriy, terx, tery;
	};

	class BOX: public GRAPH
	{
	public:
		BOX(int x1, int y1, int x2, int y2);
		void paint(HDC hDC);
	private:
		int x1, y1, x2, y2;
	};
};

class SUBWINDOW
{
public:
	~SUBWINDOW();
	static void init();
	static SUBWINDOW& getInstance(){return instance;}
	void paint(HDC hDC);
	void startPaint();
	void endPaint();

	void Letter(const char* str, int x, int y);
	void Line(int orix, int oriy, int terx, int tery);
	void Box(int x1, int y1, int x2, int y2);

private:
	SUBWINDOW();
	static SUBWINDOW instance;
	std::list<space_subwindow::GRAPH*> graphlist;
};