---
title: 'minicc 课程连续阅读'
layout: course_page
permalink: '/course/continuous/'
type: '连续阅读'
sidebar:
  nav: course-continuous
source_path: 'generated:continuous'
---

{% for page in site.course %}
  {% if page.source_path == "course/README.md" %}
<section id="course-home" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/00-orientation.md" %}
<section id="chapter-0" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/practice/README.md" %}
<section id="lab-guide" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/chapters/ch01-step1-driver.md" %}
<section id="ch01-step1-driver" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/chapters/ch02-step2-lexer.md" %}
<section id="ch02-step2-lexer" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/chapters/ch03-step3-parser.md" %}
<section id="ch03-step3-parser" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/chapters/ch04-step4-semantic.md" %}
<section id="ch04-step4-semantic" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

{% for page in site.course %}
  {% if page.source_path == "course/chapters/ch05-step5-codegen.md" %}
<section id="ch05-step5-codegen" class="chapter-section" markdown="1">
## {{ page.title }}
{{ page.content }}
</section>
  {% endif %}
{% endfor %}

<script>
document.addEventListener('DOMContentLoaded', function() {
  const sections = document.querySelectorAll('.chapter-section');
  const navLinks = document.querySelectorAll('.sidebar a[href*="#"]');
  function updateActiveLink() {
    let current = '';
    sections.forEach(section => {
      const sectionTop = section.offsetTop;
      if (window.pageYOffset >= sectionTop - 120) {
        current = section.getAttribute('id');
      }
    });
    navLinks.forEach(link => {
      link.classList.remove('active');
      if (link.getAttribute('href').includes('#' + current)) {
        link.classList.add('active');
      }
    });
  }
  window.addEventListener('scroll', updateActiveLink);
  updateActiveLink();
});
</script>
