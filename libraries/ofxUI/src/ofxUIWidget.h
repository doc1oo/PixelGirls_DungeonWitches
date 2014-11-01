/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef OFXUI_WIDGET
#define OFXUI_WIDGET

class ofxUIWidget           
{
public:
    ofxUIWidget() 
    {
        parent = NULL; 
        name = string("base");
        ID = -1;
        hit = false; 
        visible = true; 
#ifdef OFX_UI_TARGET_TOUCH
        touchId = -1; 
#endif
        state = OFX_UI_STATE_NORMAL; 
        draw_back = true; 
        draw_outline = false; 
        draw_fill = false; 
        draw_fill_highlight = false;
        draw_outline_highlight = false;         

        padding = OFX_UI_GLOBAL_PADDING; 
		draw_padded_rect = OFX_UI_DRAW_PADDING; 						
		draw_padded_rect_outline = OFX_UI_DRAW_PADDING_OUTLINE; 						        
        
        color_back = OFX_UI_COLOR_BACK;								//the rect's back color
        color_outline = OFX_UI_COLOR_OUTLINE;						//the rect's outline color 
        color_outline_highlight = OFX_UI_COLOR_OUTLINE_HIGHLIGHT;   //the rect's onMouseOver outline highlight color         
		color_fill = OFX_UI_COLOR_FILL;								//the rect's fill color 
        color_fill_highlight = OFX_UI_COLOR_FILL_HIGHLIGHT;         //the rect's onMouseDown highlight color 

        color_padded_rect = OFX_UI_COLOR_PADDED;
        color_padded_rect_outline = OFX_UI_COLOR_PADDED_OUTLINE;     
        
        embedded = false;
		modal = false;
    }
    
    virtual ~ofxUIWidget() 
    {
        if(rect != NULL)
        {
            delete rect; 
        }
        if(paddedRect != NULL)
        {
            delete paddedRect; 
        }
    }
    
    virtual void update() {}
    virtual void draw() 
    {
        ofxUIPushStyle();
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);        
        
        drawPadded();
        drawPaddedOutline();        

        drawBack();
        
        drawOutline();
        drawOutlineHighlight();
        
        drawFill();
        drawFillHighlight();
        
        ofxUIPopStyle();
    }
    
    virtual void drawBack() 
    {
        if(draw_back)
        {
            ofxUIFill();
            ofxUISetColor(color_back);
            rect->draw(); 
        }
    }
    
    virtual void drawOutline() 
    {
        if(draw_outline)
        {
            ofxUINoFill();
            ofxUISetColor(color_outline); 
            rect->draw(); 
        } 
    }
    
    virtual void drawOutlineHighlight() 
    {
        if(draw_outline_highlight)
        {
            ofxUINoFill();
            ofxUISetColor(color_outline_highlight); 
            rect->draw();          
        }
    }    
    
    virtual void drawFill() 
    {
        if(draw_fill)
        {
            ofxUIFill(); 
            ofxUISetColor(color_fill); 
            rect->draw(); 
        }
    }
    
    virtual void drawFillHighlight() 
    {
        if(draw_fill_highlight)
        {
            ofxUIFill(); 
            ofxUISetColor(color_fill_highlight); 
            rect->draw(); 
        }    
    }
    
    virtual void drawPadded()
    {
		if(draw_padded_rect && !embedded)
		{
            ofxUIFill();
            ofxUISetColor(color_padded_rect); 
			paddedRect->draw(); 
		}                
    }
    
    virtual void drawPaddedOutline()
    {
        if(draw_padded_rect_outline && !embedded)
		{
            ofxUINoFill();
            ofxUISetColor(color_padded_rect_outline); 
			paddedRect->draw(); 
		}                
    }
    
#ifdef OFX_UI_TARGET_TOUCH          //iOS Mode
    void touchDown(float x, float y, int id)
    {
        if(touchId == -1)
        {    
            this->mousePressed(x, y, 0);
            if(hit)
            {
                touchId = id;    
            }            
        }    
    }
    
    void touchMoved(float x, float y, int id)
    {
        if(touchId == id)
        {
            this->mouseDragged(x, y, 0);
        }       
    }
    
    void touchUp(float x, float y, int id)
    {
        if(touchId == id)
        {
            this->mouseReleased(x, y, 0);
            touchId = -1;                      
        }
    }
    
    void touchCancelled(float x, float y, int id)
    {
        if(touchId == id)
        {
            this->mouseReleased(x, y, 0);
            touchId = -1;                
        }
    }
    
    void touchDoubleTap(float x, float y, int id)
    {
        
    }
#endif

	virtual void mouseMoved(int x, int y ) {}
	virtual void mouseDragged(int x, int y, int button) {}
	virtual void mousePressed(int x, int y, int button) {}
	virtual void mouseReleased(int x, int y, int button) {}
    
	virtual void keyPressed(int key) {}
	virtual void keyReleased(int key) {}
	virtual void windowResized(int w, int h) {}           		
    
	virtual void setParent(ofxUIWidget *_parent)
	{
		parent = _parent; 
	}
	
	virtual void setRectParent(ofxUIRectangle *_prect)
	{
		rect->setParent(_prect); 
	}

	virtual ofxUIWidget *getParent()
	{
		return parent; 
	}

	virtual ofxUIRectangle* getRect()
	{
		return rect; 
	}
	
	virtual void setName(string _name)
	{
		name = string(_name);  
	}
    
	virtual void setState(int _state)
    {
        state = _state; 
    }
    //--------------------------------------------------------------------------------
	virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
	}    
    
	virtual void setDrawBack(bool _draw_back)
	{
		draw_back = _draw_back; 
	}

	virtual void setDrawOutline(bool _draw_outline)
	{
		draw_outline = _draw_outline; 
	}

	virtual void setDrawFill(bool _draw_fill)
	{
		draw_fill = _draw_fill; 
	}
	
	virtual void setDrawFillHighLight(bool _draw_fill_highlight)
	{
		draw_fill_highlight = _draw_fill_highlight; 
	}
	
	virtual void setDrawOutlineHighLight(bool _draw_outline_hightlight)
	{
		draw_outline_highlight = _draw_outline_hightlight; 
	}
    //--------------------------------------------------------------------------------
	virtual bool getDrawPadding()
	{
		return draw_padded_rect; 
	}
    
    virtual bool getDrawPaddingOutline()
	{
		return draw_padded_rect_outline; 
	}

	virtual bool getDrawBack()
	{
		return draw_back; 
	}
    
	virtual bool getDrawOutline()
	{
		return draw_outline; 
	}
    
	virtual bool getDrawFill()
	{
		return draw_fill; 
	}
	
	virtual bool getDrawFillHighLight()
	{
		return draw_fill_highlight; 
	}
	
	virtual bool getDrawOutlineHighLight()
	{
		return draw_outline_highlight;
	}
    
	virtual void setColorBack(ofxUIColor _color_back)
	{
		color_back = _color_back; 
	}
		
	virtual void setColorOutline(ofxUIColor _color_outline)
	{
		color_outline = _color_outline; 
	}
	
	virtual void setColorOutlineHighlight(ofxUIColor _color_outline_highlight)
	{
		color_outline_highlight = _color_outline_highlight; 
	}	

	virtual void setColorFill(ofxUIColor _color_fill)
	{
		color_fill = _color_fill; 
	}
	
	virtual void setColorFillHighlight(ofxUIColor _color_fill_highlight)
	{
		color_fill_highlight = _color_fill_highlight; 
	}
	
    virtual void setColorPadded(ofxUIColor _color_padded_rect)
    {
        color_padded_rect = _color_padded_rect; 
    }
    
    virtual void setColorPaddedOutline(ofxUIColor _color_padded_rect_outline)
    {
        color_padded_rect_outline = _color_padded_rect_outline; 
    }
    
	ofxUIColor& getColorPadded()
	{
        return color_padded_rect;
	}

	ofxUIColor& getColorPaddedOutline()
	{
        return color_padded_rect_outline;
	}
    
	ofxUIColor& getColorBack()
	{
		return color_back; 
	}
	
	ofxUIColor& getColorOutline()
	{
		return color_outline; 
	}
	
	ofxUIColor& getColorOutlineHighlight()
	{
		return color_outline_highlight; 
	}	
	
	ofxUIColor& getColorFill()
	{
		return color_fill; 
	}
	
	ofxUIColor& getColorFillHighlight()
	{
		return color_fill_highlight; 
	}
	
    virtual int getKind()
	{
		return kind; 
	}
    
	virtual void setFont(ofxUIFont *_font)
	{
		font = _font; 
	}
	
    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
    }
    
    virtual bool isVisible()
    {
        return visible; 
    }
    
    virtual void toggleVisible()
    {
        visible =! visible; 
    }
    
    virtual bool isHit(float x, float y)
    {
        if(visible)
        {
            return rect->inside(x, y);
        }
        else
        {
            return false; 
        }
    }
    
	virtual string& getName()
	{
		return name;
	}
	
	virtual void triggerEvent(ofxUIWidget *child)
	{
		if(parent != NULL)
		{
			parent->triggerEvent(child); 
		}
	}
    
    virtual void triggerSelf()
    {
		if(parent != NULL)
		{
			parent->triggerEvent(this); 
		}        
    }
	
	virtual void setPadding(float _padding)
	{
		padding = _padding; 
		paddedRect->set(-padding, -padding, rect->getWidth()+padding*2.0, rect->getHeight()+padding*2.0);
	}
	
	virtual float getPadding()
	{
		return padding; 
	}
	
	virtual ofxUIRectangle *getPaddingRect()
	{
		return paddedRect; 
	}

    virtual void stateChange()
    { 
        
    }
    
    virtual bool isDraggable()
    {
        return false; 
    }
    
    virtual bool isEmbedded()
    {
        return embedded; 
    }
    
    virtual void setEmbedded(bool _embedded)
    {
        embedded = _embedded; 
    }
    
    void setID(int _id)
    {
        ID = _id;
    }
    
    int getID()
    {
        return ID;
    }
    
    virtual void addWidget(ofxUIWidget *widget)
    {
        //Experimental
    }    

    virtual void removeWidget(ofxUIWidget *widget)
    {
        //Experimental
    }    
    
    virtual bool hasLabel()
    {
        return false; 
    }
    
    virtual bool isModal()
    {
        return modal;
    }
    
    virtual void setModal(bool _modal)      //allows for piping mouse/touch input to widgets that are outside of parent's rect/canvas
    {
        modal = _modal;
        if(parent != NULL)
        {
            if(modal)
            {
                parent->addModalWidget(this);
            }
            else
            {
                parent->removeModalWidget(this);
            }
        }
    }
    
    virtual void addModalWidget(ofxUIWidget *widget)
    {
        if(parent != NULL)
        {
            parent->addModalWidget(widget);
        }
    }
    
    virtual void removeModalWidget(ofxUIWidget *widget)
    {
        if(parent != NULL)
        {
            parent->removeModalWidget(widget);
        }
    }
    
    virtual void addEmbeddedWidget(ofxUIWidget *widget)
    {
        widget->setEmbedded(true);
        embeddedWidgets.push_back(widget);        
    }
    
    virtual int getEmbeddedWidgetsSize()
    {
        return embeddedWidgets.size();
    }
    
    ofxUIWidget *getEmbeddedWidget(int index)
    {
        return embeddedWidgets[index%embeddedWidgets.size()];
    }
    
    virtual void clearEmbeddedWidgets()
    {
        embeddedWidgets.clear();        //does not deallocate widgets, just deletes the pointers and sets the size to zero
    }
    
    ofxUIWidget *getCanvasParent()
    {
        bool notFoundParentCanvas = true;
        ofxUIWidget *parent = this->getParent();
        
        while (notFoundParentCanvas)
        {
            int kind = parent->getKind();
            if( kind == OFX_UI_WIDGET_CANVAS || kind == OFX_UI_WIDGET_SCROLLABLECANVAS || kind == OFX_UI_WIDGET_SUPERCANVAS )
            {
                notFoundParentCanvas = false;
                return parent; 
            }
            else
            {
                parent = parent->getParent();
            }
        }
    }
    
protected:
	ofxUIWidget *parent;
	ofxUIRectangle *rect;
	ofxUIFont *font;
	
    string name;            //State Properties
	int kind; 
	bool visible;
    int ID;
    bool hit; 
    int state; 
    bool embedded;
    bool modal;
    
	bool draw_back;         //Rendering Properties 
	bool draw_outline; 
	bool draw_outline_highlight; 
	bool draw_fill; 
	bool draw_fill_highlight; 

	ofxUIColor color_back;
	ofxUIColor color_outline; 
	ofxUIColor color_outline_highlight;	
	ofxUIColor color_fill; 
	ofxUIColor color_fill_highlight; 

	float padding;          //Spacing/Padding Purposes
	ofxUIRectangle *paddedRect; 	

	bool draw_padded_rect; 
	bool draw_padded_rect_outline;     
    ofxUIColor color_padded_rect; 
	ofxUIColor color_padded_rect_outline;
    
    vector<ofxUIWidget *> embeddedWidgets; 
    
#ifdef OFX_UI_TARGET_TOUCH          //iOS Mode
    int touchId;     
#endif
};

#endif
