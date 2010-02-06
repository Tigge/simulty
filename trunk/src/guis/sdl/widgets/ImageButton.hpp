#ifndef _IMAGEBUTTON_HPP_
#define _IMAGEBUTTON_HPP_

#include <guichan.hpp>

namespace gcn {
  
  class ImageButton : public Button {
  
    private:
    
    Image *image;
    
    public:
    
    virtual void setImage(Image *image) {
      this->image = image;
    }
    virtual void draw (Graphics *graphics) {
      Button::draw(graphics);
      graphics->drawImage(image, 0, 0, 1, 1, getWidth() - 2, getHeight() - 2);
    }
    
    virtual void adjustSize() {
      setWidth((this->image)->getWidth() + 2);
      setHeight(this->image->getHeight() + 2);
    }
    
    ImageButton(Image *image) : Button("") {
      setImage(image);
      adjustSize();
    }
    virtual ~ImageButton() {
      
    }
  };
}

#endif
