import pygame
import random
import sys
import math

# Initialize Pygame
pygame.init()
pygame.font.init()

# Game Constants
SCREEN_WIDTH = 500
SCREEN_HEIGHT = 700
FPS = 60

# Colors (Hex/RGB palette)
COLOR_SKY = (34, 40, 49)       # Dark Slate
COLOR_BIRD = (253, 112, 20)     # Vibrant Orange
COLOR_PIPE = (46, 204, 113)     # Emerald Green
COLOR_PIPE_LIP = (39, 174, 96) # Dark Emerald
COLOR_TEXT = (238, 238, 238)    # Clean White
COLOR_PARTICLE = (255, 211, 105)# Gold Accent

class Bird:
    def __init__(self):
        self.x = 100
        self.y = SCREEN_HEIGHT // 2
        self.radius = 18
        self.velocity = 0
        self.gravity = 0.5
        self.jump_strength = -8.5
        self.angle = 0

    def jump(self):
        self.velocity = self.jump_strength

    def update(self):
        # Apply Gravity
        self.velocity += self.gravity
        self.y += self.velocity

        # Cap terminal velocity
        if self.velocity > 12:
            self.velocity = 12

        # Rotate bird based on velocity for advanced visual effect
        self.angle = -self.velocity * 4
        self.angle = max(-30, min(70, self.angle)) # Limit rotation

    def draw(self, surface):
        # Create a separate surface for rotation to keep drawing clean
        bird_surf = pygame.Surface((self.radius * 2, self.radius * 2), pygame.SRCALPHA)
        
        # Body
        pygame.draw.circle(bird_surf, COLOR_BIRD, (self.radius, self.radius), self.radius)
        # Eye
        pygame.draw.circle(bird_surf, (255, 255, 255), (self.radius + 8, self.radius - 4), 5)
        pygame.draw.circle(bird_surf, (0, 0, 0), (self.radius + 10, self.radius - 4), 2)
        # Beak
        pygame.draw.polygon(bird_surf, (255, 168, 1), [(self.radius*2 - 2, self.radius - 4), (self.radius*2 + 6, self.radius), (self.radius*2 - 2, self.radius + 4)])

        # Rotate the surface
        rotated_surf = pygame.transform.rotate(bird_surf, self.angle)
        new_rect = rotated_surf.get_rect(center=(self.x, self.y))
        
        surface.blit(rotated_surf, new_rect.topleft)


class Pipe:
    def __init__(self, x):
        self.x = x
        self.gap = 160
        self.width = 80
        self.top_height = random.randint(100, SCREEN_HEIGHT - self.gap - 100)
        self.bottom_y = self.top_height + self.gap
        self.bottom_height = SCREEN_HEIGHT - self.bottom_y
        self.speed = 3.5
        self.passed = False

    def update(self):
        self.x -= self.speed

    def draw(self, surface):
        # Top Pipe
        top_rect = pygame.Rect(self.x, 0, self.width, self.top_height)
        pygame.draw.rect(surface, COLOR_PIPE, top_rect)
        pygame.draw.rect(surface, COLOR_PIPE_LIP, (self.x - 4, self.top_height - 20, self.width + 8, 20), border_radius=3)

        # Bottom Pipe
        bottom_rect = pygame.Rect(self.x, self.bottom_y, self.width, self.bottom_height)
        pygame.draw.rect(surface, COLOR_PIPE, bottom_rect)
        pygame.draw.rect(surface, COLOR_PIPE_LIP, (self.x - 4, self.bottom_y, self.width + 8, 20), border_radius=3)

    def collide(self, bird):
        bird_rect = pygame.Rect(bird.x - bird.radius, bird.y - bird.radius, bird.radius*2, bird.radius*2)
        top_rect = pygame.Rect(self.x, 0, self.width, self.top_height)
        bottom_rect = pygame.Rect(self.x, self.bottom_y, self.width, self.bottom_height)

        if bird_rect.colliderect(top_rect) or bird_rect.colliderect(bottom_rect):
            return True
        return False


class ParticleSystem:
    def __init__(self):
        self.particles = []

    def emit(self, x, y):
        self.particles.append([[x, y], [random.randint(-4, -1), random.randint(-2, 2)], random.randint(3, 6)])

    def update_and_draw(self, surface):
        for particle in self.particles[:]:
            particle[0][0] += particle[1][0] 
            particle[0][1] += particle[1][1] 
            particle[2] -= 0.15 
            
            if particle[2] <= 0:
                self.particles.remove(particle)
            else:
                pygame.draw.circle(surface, COLOR_PARTICLE, (int(particle[0][0]), int(particle[0][1])), int(particle[2]))


class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        pygame.display.set_caption("Bird Flap")
        self.clock = pygame.time.Clock()
        
        # Fonts
        self.font_title = pygame.font.SysFont("Helvetica", 64, bold=True)
        self.font_score = pygame.font.SysFont("Helvetica", 48, bold=True)
        self.font_ui = pygame.font.SysFont("Helvetica", 24)
        
        self.reset_game()

    def reset_game(self):
        self.bird = Bird()
        self.pipes = [Pipe(SCREEN_WIDTH + 100)]
        self.particles = ParticleSystem()
        self.score = 0
        self.game_active = False
        self.game_over = False

    def run(self):
        while True:
            self.clock.tick(FPS)
            self.handle_events()
            
            if self.game_active:
                self.update_game()
                
            self.draw_screen()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
                
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    if not self.game_active and not self.game_over:
                        self.game_active = True
                    if self.game_active:
                        self.bird.jump()
                        for _ in range(5):
                            self.particles.emit(self.bird.x, self.bird.y)
                    if self.game_over:
                        self.reset_game()

    def update_game(self):
        self.bird.update()
        
        if random.random() > 0.4:
            self.particles.emit(self.bird.x - 10, self.bird.y)

        pipe_to_remove = None
        for pipe in self.pipes:
            pipe.update()
            
            if pipe.collide(self.bird):
                self.game_active = False
                self.game_over = True

            if not pipe.passed and pipe.x < self.bird.x:
                pipe.passed = True
                self.score += 1

            if pipe.x < -pipe.width:
                pipe_to_remove = pipe

        if pipe_to_remove:
            self.pipes.remove(pipe_to_remove)

        if len(self.pipes) > 0 and self.pipes[-1].x < SCREEN_WIDTH - 250:
            self.pipes.append(Pipe(SCREEN_WIDTH))

        if self.bird.y - self.bird.radius <= 0 or self.bird.y + self.bird.radius >= SCREEN_HEIGHT:
            self.game_active = False
            self.game_over = True

    def draw_screen(self):
        self.screen.fill(COLOR_SKY)
        
        # Structrual Grid
        for i in range(0, SCREEN_HEIGHT, 40):
            pygame.draw.line(self.screen, (40, 48, 58), (0, i), (SCREEN_WIDTH, i), 1)

        self.particles.update_and_draw(self.screen)
        
        for pipe in self.pipes:
            pipe.draw(self.screen)
            
        self.bird.draw(self.screen)

        # --- MENU OVERLAYS ---
        
        # Main Start Screen
        if not self.game_active and not self.game_over:
            # Main "Bird Flap" Title Accent Header
            self.draw_overlay_text("BIRD FLAP", SCREEN_HEIGHT // 3, font_type="title", color=COLOR_BIRD)
            # Action Subtext
            self.draw_overlay_text("PRESS SPACE TO FLAP", SCREEN_HEIGHT // 2, font_type="ui")
            
        # Game Over Screen
        if self.game_over:
            self.draw_overlay_text("GAME OVER", SCREEN_HEIGHT // 3, font_type="score")
            self.draw_overlay_text(f"FINAL SCORE: {self.score}", SCREEN_HEIGHT // 2, font_type="ui")
            self.draw_overlay_text("PRESS SPACE TO RESTART", SCREEN_HEIGHT // 1.5, font_type="ui")

        # Active Gameplay HUD
        if self.game_active:
            score_surface = self.font_score.render(str(self.score), True, COLOR_TEXT)
            self.screen.blit(score_surface, (SCREEN_WIDTH - score_surface.get_width() - 30, 20))

        pygame.display.flip()

    def draw_overlay_text(self, text, y_pos, font_type="ui", color=COLOR_TEXT):
        # Pick correct font weight
        if font_type == "title":
            font = self.font_title
        elif font_type == "score":
            font = self.font_score
        else:
            font = self.font_ui

        text_surface = font.render(text, True, color)
        
        # Background container panel
        bg_rect = pygame.Rect(0, 0, text_surface.get_width() + 30, text_surface.get_height() + 10)
        bg_rect.center = (SCREEN_WIDTH // 2, y_pos)
        pygame.draw.rect(self.screen, (25, 30, 36, 180), bg_rect, border_radius=8)
        
        text_rect = text_surface.get_rect(center=(SCREEN_WIDTH // 2, y_pos))
        self.screen.blit(text_surface, text_rect)


if __name__ == "__main__":
    game = Game()
    game.run()
