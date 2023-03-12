#include "Camera.hpp"

namespace gps {
    
    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
    
        //TODO - Update the rest of camera parameters
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraTarget, cameraUpDirection);
    }

    
    glm::vec3 Camera::mycamera()
    {
        return (this->cameraPosition);
    }


    glm::vec3 cameraForward, cameraRight;
    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        
        cameraForward = normalize(this->cameraTarget - this->cameraPosition);
        cameraRight = normalize(cross(cameraForward, this->cameraUpDirection));
        this->cameraPosition = this->cameraPosition;
        if(direction== MOVE_FORWARD)
            this->cameraPosition += this->cameraTarget * speed;
        if(direction== MOVE_BACKWARD)
            this->cameraPosition -= this->cameraTarget * speed;
       if (direction == MOVE_LEFT)
            this->cameraPosition -= glm::normalize(glm::cross(cameraTarget, this->cameraUpDirection)) * speed;
       if (direction == MOVE_RIGHT)
            this->cameraPosition += glm::normalize(glm::cross(cameraTarget, this->cameraUpDirection)) * speed;
       if (direction == MOVE_UP)
            this->cameraPosition.y += speed;
       if (direction == MOVE_DOWN)
            this->cameraPosition.y -= speed;
           
           
        
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 cam;
        cam.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cam.y = sin(glm::radians(pitch));
        cam.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraTarget = glm::normalize(cam);
        
    }
}