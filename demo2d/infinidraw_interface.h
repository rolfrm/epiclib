class QuadTree;
class QuadtreeRenderer;
class Texture2DShader;
extern QuadTree * currentTree;
extern QuadtreeRenderer * QuadTreeRenderer;
extern Texture2DShader * texShader;
void init_infinidraw();
void infinidraw_render();
void SetBrushSize(double Scale);
void SetColor(float r, float g, float b, float a);
void MoveCamera(double dx, double dy);
void Zoom(double amount, int aroundX, int aroundY);
int GetColorAt(int screenX, int screenY);
void PaintAt(int screenX, int screenY);
void SetScreenSize(int screenWidth, int screenHeight);
void PaintDone();

